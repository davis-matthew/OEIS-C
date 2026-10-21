/*
 * A051173  Triangle read by rows: T(n, k) = lcm(n, k).
 * https://oeis.org/A051173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the flattened triangle with entries * T(r,k) = lcm(r,k).
 *            The index n maps to row r and column k * where r is the smallest
 *            integer with r(r+1)/2 ≥ n and * k = n – (r−1)r/2. Then a(n) =
 *            lcm(r,k) = r/gcd(r,k)*k
 * Range:     u8 n <= 151, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t (works for built‑in or big‑int types). */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the row number r (1‑based) that contains index n. */
static inline idx_t row_of(idx_t n)
{
    idx_t r = 1;
    __int128 tri = 1;               /* r*(r+1)/2 for r==1 */
    while (tri < (__int128)n)
    {
        ++r;
        tri = (__int128)r * (r + 1) / 2;
    }
    return r;
}

/* Main function required by the grader. */
seq_t a(idx_t n)
{
    /* Find the (row, column) of the flattened index n. */
    idx_t r = row_of(n);
    __int128 prev = (__int128)(r - 1) * r / 2;   /* elements before row r */
    idx_t k = n - (idx_t)prev;                  /* 1‑based column */

    /* Compute lcm(r,k). */
    seq_t a_r = (seq_t)r;
    seq_t a_k = (seq_t)k;
    seq_t g   = gcd_seq_t(a_r, a_k);
    return (a_r / g) * a_k;
}
