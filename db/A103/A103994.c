/*
 * A103994  Triangle read by rows: A129360 * A115361 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A103994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Validated: standard, pass, 107 terms through n = 1177 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A103994: Triangle read by rows, entries T(r,k) = A209635(r/k) for k|r;
    a(n) is the nth element of that triangle (1‑based).  */
#include <stdint.h>

/*  A209635(m): 0 if m has an odd squared prime factor;
    otherwise 1 or –1 depending on the parity of the number of odd prime factors. */
static seq_t a209635(idx_t m)
{
    if (m <= 1) return (seq_t)1;          /* m = 1 → 1 */

    /* strip factors of 2 – they are irrelevant for square‑freeness test */
    while ((m & 1) == 0) m >>= 1;

    int parity = 0;   /* 0 = even number of odd primes, 1 = odd */

    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1) return (seq_t)0;   /* odd factor squared */
            }
            parity ^= 1;  /* odd prime contributes to parity */
        }
    }
    if (m > 1) parity ^= 1; /* remaining odd prime factor */

    return parity ? (seq_t)-1 : (seq_t)1;
}

/* Locate the row r (1‑based) such that r(r+1)/2 ≥ n.  */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 1, high = 1;
    /* increase high until it is large enough */
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n ≤ 0 */

    idx_t r = row_of_index(n);
    idx_t tri_prev = r * (r - 1) / 2;        /* (r-1)r/2 */
    idx_t k = n - tri_prev;                  /* column within the row */

    if (r % k != 0) return (seq_t)0;         /* k does not divide r */

    idx_t m = r / k;
    return a209635(m);
}
