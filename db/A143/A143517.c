/*
 * A143517  Triangle read by rows, A054525 * (A061397 * 0^(n-k)), 1<=k<=n.
 * https://oeis.org/A143517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = value read from the triangle defined in OEIS A143517 The
 *            triangle entry t(n,k) (1 ≤ k ≤ n) is t(n,k) = 0 if k is not
 *            prime = k if k = n (and k prime) = 0 if n % k ≠ 0 = 0 if (n/k)
 *            is a perfect square = (-1)^(Ω(n/k)) * k otherwise where Ω(m) is
 *            the total number of prime factors of m (with m...
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   omega_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/omega_uint.h"

/* ---- local helpers ---- */
static bool is_prime_uint(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t p = 5; p * p <= n; p += 6) {
        if (n % p == 0) return false;
        if (n % (p + 2) == 0) return false;
    }
    return true;
}

static bool is_square_uint(uint64_t n)
{
    uint64_t r = (uint64_t)sqrtl((long double)n);
    while (r * r < n) ++r;
    while (r * r > n) --r;
    return r * r == n;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */
    /* find row r such that r*(r+1)/2 >= n */
    uint64_t r = (uint64_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* adjust for rounding errors */
    while (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && ((unsigned __int128)(r - 1) * r) / 2 >= n) --r;

    uint64_t prev = ((unsigned __int128)(r - 1) * r) / 2;
    uint64_t k = n - prev;                 /* column index, 1‑based */

    if (k < 2) return 0;                   /* k must be prime >1 */
    if (!is_prime_uint(k)) return 0;       /* only prime k can contribute */

    if (k == r) return (seq_t)k;           /* diagonal prime entries */

    if (r % k != 0) return 0;              /* k must divide r */

    uint64_t q = r / k;
    if (is_square_uint(q)) return 0;        /* perfect square quotient gives 0 */

    int omega = omega_uint(q);
    if ((omega & 1) == 0)                  /* even Ω → positive */
        return (seq_t)k;
    else                                   /* odd Ω → negative */
        return -(seq_t)k;
}
