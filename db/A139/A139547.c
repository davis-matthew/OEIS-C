/*
 * A139547  Triangle read by rows: T(n,k) = A003418(A010766).
 * https://oeis.org/A139547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  T(r,c) = lcm(1,2,…,⌊(r+1)/c⌋)   (0‑based row r, 1‑based column c)
    a(n) is the flattened triangle read by rows (offset 0). */

#include <stdbool.h>

/* Euclidean algorithm for the GCD of two seq_t numbers. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/*  Compute a(n) for the OEIS sequence A139547 . */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column position (1‑based) within the row. */
    idx_t col = n - r * (r + 1) / 2 + 1;

    /* The argument of the lcm function. */
    idx_t m = (r + 1) / col;   /* floor((r+1)/col) */

    /* Compute lcm(1..m) efficiently. */
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t g   = gcd_seq(res, (seq_t)i);
        idx_t  gI = (idx_t)g;           /* g divides i, so it fits in idx_t. */
        idx_t  mult = i / gI;
        res = res * (seq_t)mult;
    }
    return res;
}
