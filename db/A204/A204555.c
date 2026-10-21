/*
 * A204555  The number of subsets of the numbers {1,2,3...,n} consisting of at
 *          most 3 elements and at most two of those are even.
 * https://oeis.org/A204555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + C(n,2) + (C(n,3) - C(floor(n/2),3))
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 1;                 /* empty subset */
    seq_t n1 = (seq_t)n;             /* cast index to seq_t for algebra */

    total += n1;                     /* subsets of size 1 */

    if (n >= 2) {                    /* subsets of size 2 */
        seq_t c2 = n1 * (n1 - 1) / 2;
        total += c2;
    }

    if (n >= 3) {                    /* subsets of size 3 */
        seq_t c3 = n1 * (n1 - 1) * (n1 - 2) / 6;
        total += c3;
    }

    seq_t e = n1 / 2;                /* number of even elements in {1..n} */
    if (e >= 3) {                    /* subtract subsets of 3 evens */
        seq_t ce3 = e * (e - 1) * (e - 2) / 6;
        total -= ce3;
    }

    return total;
}
