/*
 * A058214  Sum of solutions of phi(x) = 2^n.
 * https://oeis.org/A058214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ{ x | φ(x) = 2ⁿ } (sum of all solutions of φ(x)=2ⁿ) Each
 *            solution has the form x = 2ᵏ · Πpᵢ where each pᵢ is a Fermat
 *            prime 3,5,17,257,65537. Let S = Σ₂ᵐ be the sum of the exponents
 *            of 2 in pᵢ−1 for the chosen subset. Then if S = n → k = 0 or 1,
 *            contribution 3·Πpᵢ if S < n → k = n − S + 1...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Fermat primes and the exponents of 2 in p−1 (3,5,17,257,65537) */
static const uint64_t fermat_primes[5] = {3ULL,5ULL,17ULL,257ULL,65537ULL};
static const int     fermat_exponents[5] = {1,2,4,8,16};

/* For every subset (mask 0..31) store the product of the chosen primes
   and the sum S of the corresponding exponents. */
static int           subset_sum[1<<5];
static uint64_t      subset_prod[1<<5];
static int          subset_initialized = 0;

/* Compute the arrays above once. */
static void init_subsets(void)
{
    for (int mask = 0; mask < (1<<5); ++mask) {
        int s = 0;
        uint64_t p = 1;
        for (int i = 0; i < 5; ++i) {
            if (mask & (1<<i)) {
                s += fermat_exponents[i];
                p *= fermat_primes[i];
            }
        }
        subset_sum[mask]  = s;
        subset_prod[mask] = p;
    }
    subset_initialized = 1;
}

/* Cache powers of two in a dynamic array.  pow2[e] = 2^e as seq_t. */
static seq_t *pow2 = NULL;
static size_t pow2_len = 0;

static void ensure_pow2(idx_t up_to)
{
    if (pow2_len > up_to) return;          /* already computed */

    size_t new_len = up_to + 1;            /* up to and including up_to */
    seq_t *new_pow = (seq_t*)realloc(pow2, new_len * sizeof(seq_t));
    if (!new_pow) return;                  /* out of memory – results may be wrong */

    pow2 = new_pow;
    if (pow2_len == 0) {
        pow2[0] = (seq_t)1;
        pow2_len = 1;
    }
    for (size_t i = pow2_len; i < new_len; ++i) {
        pow2[i] = pow2[i-1] * (seq_t)2;
    }
    pow2_len = new_len;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!subset_initialized) init_subsets();

    /* we need powers up to exponent n+1 (k = n−S+1 can be as large as n+1) */
    ensure_pow2(n + 1);

    seq_t total = 0;

    for (int mask = 0; mask < (1<<5); ++mask) {
        int s = subset_sum[mask];
        if ((idx_t)s > n) continue;           /* this subset cannot satisfy φ(x)=2^n */

        seq_t prod = (seq_t)subset_prod[mask];

        if ((idx_t)s == n) {                   /* S = n, k = 0 or 1 */
            total += prod * (seq_t)3;           /* 1·P + 2·P = 3·P */
        } else {                               /* S < n, k = n−S+1 ≥ 2 */
            idx_t k = n - (idx_t)s + 1;
            seq_t powk = pow2[k];
            total += prod * powk;
        }
    }
    return total;
}
