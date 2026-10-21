/*
 * A068386  One-thirtieth the area of the unique Pythagorean triangle whose
 *          hypotenuse is A002144(n), the n-th prime of the form 4k+1.
 * https://oeis.org/A068386
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = rs(s^2 - r^2) / 30, where the n‑th prime ≡1 (mod 4) is
 *            uniquely written as r² + s² (1 ≤ r < s). The function returns
 *            the integer value a(n). The sequence is offset 2: a(2)=1,
 *            a(3)=2, …
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Store primes ≡1 (mod 4) up to a fixed limit (enough for n≈10000). */
static uint64_t *primes4k1 = NULL;   /* array of primes such that p%4==1 */
static size_t primes4k1_cnt = 0;     /* number of stored primes */

/* Integer square‑root using long double – accurate for our range. */
static uint64_t int_sqrt(uint64_t n) {
    uint64_t r = (uint64_t)sqrtl((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Initialise primes4k1 once.  Uses a sieve up to 400000, which
   provides more than 18000 primes ≡1 (mod 4). */
static void init_primes4k1(void) {
    if (primes4k1_cnt > 0) return;          /* already initialised */

    const uint32_t LIMIT = 400000;           /* safe bound for n≈10000 */
    bool *is_comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(is_comp, 0, (LIMIT + 1) * sizeof(bool));

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                is_comp[(size_t)j] = true;
        }
    }

    /* Count needed size */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i] && (i & 3) == 1) ++cnt;

    primes4k1 = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    primes4k1_cnt = cnt;
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i] && (i & 3) == 1)
            primes4k1[pos++] = i;

    free(is_comp);
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n≥2 */

    init_primes4k1();              /* ensure primes list is ready */

    /* The OEIS uses offset 2: a(2) is the first listed term, so
       we need the (n-1)-th 1 (mod 4) prime (with 0‑based indexing). */
    uint64_t p = primes4k1[n - 1];

    /* Find the unique representation p = r² + s², 1 ≤ r < s. */
    uint64_t r = 0, s = 0;
    uint64_t sqrtp = int_sqrt(p);
    for (uint64_t rr = 1; rr <= sqrtp; ++rr) {
        uint64_t r2 = rr * rr;
        if (r2 >= p) break;
        uint64_t s2 = p - r2;
        uint64_t ss = int_sqrt(s2);
        if (ss * ss == s2 && rr < ss) {
            r = rr;
            s = ss;
            break;
        }
    }

    /* Compute area = r * s * (s² - r²) and divide by 30. */
    unsigned __int128 area = (unsigned __int128)r * s *
                             (((unsigned __int128)s * s) -
                              ((unsigned __int128)r * r));
    unsigned __int128 a_val = area / 30;
    return (seq_t)a_val;
}
