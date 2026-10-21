/*
 * A053981  A fourth order generalization of the Mobius function of n.
 * https://oeis.org/A053981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053981 – A fourth order generalization of the Möbius function.
   Formula:
     a(n) = 0   if any prime exponent in n exceeds 4
     a(n) = -1  if n is exactly p^4 for some prime p
     a(n) = 1   otherwise  (including n = 1)
   The function is multiplicative with the local rule above.  It uses a
   static smallest‑prime‑factor sieve for efficient factorisation. */
#include <stdint.h>
#include <stdlib.h>

static idx_t   spf_size = 0;
static uint32_t *spf = NULL;

/* Initialise smallest prime factor array up to n (inclusive). */
static void init_spf(idx_t n)
{
    if (n <= spf_size) return;
    uint32_t *new_spf = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!new_spf) return;                       /* abort silently – unlikely */
    for (idx_t i = 0; i <= n; ++i) new_spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (new_spf[i] == 0) {                  /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    if (spf) free(spf);
    spf      = new_spf;
    spf_size = n;
}

/* Return the A053981 term for n (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 1) return 1;                       /* by definition */
    init_spf(n);
    idx_t           x = n;
    int             distinct_cnt = 0;
    int             has_shift4   = 0;           /* any exponent == 4 */

    while (x > 1) {
        uint32_t p = spf[x];
        if (p == 0) p = (uint32_t)x;            /* prime > sqrt(original n) */
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        if (exp > 4) return 0;                  /* exponent too large */
        if (exp == 4) has_shift4 = 1;
        ++distinct_cnt;
    }

    if (distinct_cnt == 1 && has_shift4) return -1;  /* pure fourth power */
    return 1;                                       /* otherwise */
}
