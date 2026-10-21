/*
 * A065568  Sum over all subsets of {1,..,n} of the GCD of the subset.
 * https://oeis.org/A065568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{d=1..n} φ(d) * (2^{⌊n/d⌋} – 1)
 * where φ is Euler's totient function.
 * The function uses a linear sieve for φ and fast binary exponentiation
 * for 2^{k} in the invocable seq_t type.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- helper: fast exponentiation of 2 in seq_t ---------- */
static seq_t pow2_seq(idx_t exp)
{
    seq_t result = 1;   /* sequence value 1 */
    seq_t base   = 2;   /* sequence value 2 */

    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* ---------- helper: linear sieve for φ up to n ---------- */
static uint64_t *compute_phi(idx_t n)
{
    if (n < 1) return NULL;

    uint64_t *phi = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    unsigned char *is_comp = (unsigned char*)malloc((n + 1) * sizeof(unsigned char));
    if (!phi || !is_comp) {
        free(phi);
        free(is_comp);
        return NULL;
    }

    for (idx_t i = 0; i <= n; ++i) {
        phi[i] = 0;
        is_comp[i] = 0;
    }
    phi[1] = 1;

    /* we cannot use a variable‑length array of idx_t; allocate dynamically */
    idx_t *primes = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!primes) {
        free(phi);
        free(is_comp);
        return NULL;
    }
    idx_t primes_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            phi[i] = i - 1;
        }
        for (idx_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t mul = i * p;
            if (mul > n) break;
            is_comp[mul] = 1;
            if (i % p == 0) {
                phi[mul] = phi[i] * p;
                break;
            } else {
                phi[mul] = phi[i] * (p - 1);
            }
        }
    }

    free(primes);
    free(is_comp);
    return phi;   /* caller must free the array */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1, but guard against 0 */

    uint64_t *phi = compute_phi(n);
    if (!phi) return 0;       /* allocation failed – return 0 as safe value */

    seq_t res = 0;            /* running total */

    for (idx_t d = 1; d <= n; ++d) {
        idx_t e = n / d;            /* floor(n/d) */
        seq_t term = (seq_t)phi[d];
        term *= (pow2_seq(e) - (seq_t)1);
        res += term;
    }

    free(phi);
    return res;
}
