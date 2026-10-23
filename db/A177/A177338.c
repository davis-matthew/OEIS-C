/*
 * A177338  Triangle read by rows: T(n,k) = 2^(n-tau(k)) mod n, where tau(.)
 *          is the number of divisors of its argument.
 * https://oeis.org/A177338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r − τ(k)) mod r, where n is the n‑th entry of the
 *            triangle read by rows, r is the row number, k is the column, and
 *            τ(k) is the number of positive divisors of k
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count the number of divisors of x (x ≥ 1). */
static idx_t count_divisors(idx_t x)
{
    idx_t cnt = 1;
    idx_t tmp = x;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (tmp > 1)   /* remaining prime factor */
        cnt *= 2;
    return cnt;
}

/* Find the row r such that r*(r−1)/2 < n ≤ r*(r+1)/2.  r ≥ 1. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the n‑th term of the triangle. */
seq_t a(idx_t n)
{
    /* 1. Locate the row r and column k. */
    idx_t r = find_row(n);
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;   /* 1 ≤ k ≤ r */

    /* 2. Compute τ(k). */
    idx_t tau_k = count_divisors(k);

    /* 3. Compute exponent exp = r − τ(k). */
    idx_t exp = r - tau_k;   /* exp is always non‑negative */

    /* 4. Compute 2^exp mod r. */
    if (r == 1)                    /* anything mod 1 is 0 */
        return (seq_t)0;

    seq_t base = (seq_t)2;
    seq_t mod  = (seq_t)r;
    seq_t result = 1 % mod;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}
