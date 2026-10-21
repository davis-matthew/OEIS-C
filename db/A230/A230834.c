/*
 * A230834  Loeschian semiprimes: semiprimes of the form x^2 + x*y + y^2.
 * https://oeis.org/A230834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th semiprime of the form x^2 + x*y + y^2
 *            (Loeschian semiprimes). There is no closed formula, so we
 *            compute the values by testing consecutive integers until we have
 *            found the requested number of terms
 * Range:     u8 n <= 20, u16 n <= 3376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve up to a fixed small limit ---------- */
#define PRIMES_MAX 200000U               /* sufficient for the first 10000 terms */

static uint64_t primes[PRIMES_MAX];
static size_t primes_cnt = 0;

static void
init_primes(void)
{
    if (primes_cnt != 0)
        return;                           /* already built */

    bool *is_composite = (bool *)calloc(PRIMES_MAX + 1, sizeof(bool));
    if (!is_composite) { abort(); }

    size_t idx = 0;
    for (uint64_t p = 2; p <= PRIMES_MAX; ++p) {
        if (!is_composite[p]) {
            primes[idx++] = p;
            if (p * p <= PRIMES_MAX) {
                for (uint64_t m = p * p; m <= PRIMES_MAX; m += p)
                    is_composite[m] = true;
            }
        }
    }
    primes_cnt = idx;
    free(is_composite);
}

/* ---------- Test whether a number is a Loeschian semiprime ---------- */
static bool
is_loeschian_semiprime(seq_t n)
{
    seq_t temp = n;
    size_t factor_cnt = 0;                 /* total number of prime factors (with multiplicity) */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t sp = (seq_t)p;
        if (sp * sp > temp)              /* temp is 1 or prime */
            break;

        if (temp % sp == 0) {
            int exp = 0;
            while (temp % sp == 0) {
                temp /= sp;
                ++exp;
                ++factor_cnt;
                if (factor_cnt > 2)          /* more than two prime factors – not semiprime */
                    return false;
            }
            if ((p % 3ULL) == 2 && (exp & 1))  /* odd exponent for a prime ≡ 2 (mod 3) */
                return false;
        }
    }

    if (temp > 1) {                        /* remaining factor is a prime > sqrt(original n) */
        ++factor_cnt;
        if (factor_cnt != 2)              /* we must still have exactly two factors total */
            return false;
        if ((temp % 3ULL) == 2)           /* check residue of the remaining prime */
            return false;
    } else {
        if (factor_cnt != 2) /* cooler case: n was p^2 */
            return false;
    }

    return true;
}

/* ---------- Static storage for computed terms ---------- */
#define MAX_TERMS 20000U                     /* enough for n up to ~10000 and more */
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;

/* Generate terms until we have at least n terms */
static void
generate_until(size_t n)
{
    if (terms_len >= n)
        return;

    init_primes();                           /* ensure primes are ready */

    seq_t candidate = 4;                     /* smallest Loeschian semiprime */
    if (terms_len > 0)
        candidate = terms[terms_len - 1] + 1; /* start after the last found term */

    while (terms_len < n) {
        if (is_loeschian_semiprime(candidate)) {
            if (terms_len < MAX_TERMS) {
                terms[terms_len++] = candidate;
                candidate = candidate + 1;
            } else {                       /* too many terms requested – abort */
                abort();
            }
        } else {
            candidate = candidate + 1;
        }
    }
}

/* ---------- Public user function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                           /* offset is 1 – no defined term at 0 */

    if ((size_t)n > MAX_TERMS)
        abort();                            /* cannot handle such large n with this buffer */

    generate_until((size_t)n);

    return terms[(size_t)n - 1];
}
