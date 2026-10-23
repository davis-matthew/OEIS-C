/*
 * A142971  Triangle read by rows: A061397 with negative signs interleaved
 *          with (k-1) zeros.
 * https://oeis.org/A142971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Let r be the row number such that T(r‑1) < n ≤ T(r) with T(r) =
 *            r·(r+1)/2. Let c = n – T(r‑1). If r % c == 0 and p = r / c is
 *            prime, then a(n) = –p; otherwise a(n) = 0
 * Range:     i8 n <= 117, i16 n <= 117, i32 n <= 117, i64 n <= 117,
 *            i128 n <= 117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x is prime (x ≥ 0). */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Find the row number r such that T(r‑1) < n ≤ T(r). */
static idx_t triangle_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Find an upper bound for r. */
    while ((high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is defined for n ≥ 1 */
    idx_t r   = triangle_row(n);
    idx_t offset = (r - 1) * r / 2; /* T(r‑1) */
    idx_t c   = n - offset;         /* column (1‑based) */

    /* Check if column corresponds to a prime factor. */
    if (r % c != 0) return 0;
    idx_t p = r / c;
    if (!is_prime(p)) return 0;

    return -(seq_t)p;
}
