/*
 * A225002  Duplicate of A047290.
 * https://oeis.org/A225002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 7*floor((n-1)/3) + {1,4,6}[ (n-1) mod 3 ]
 * Range:     i8 n <= 55, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   i.e. a(1)=1, a(2)=4, a(3)=6, a(4)=8, …   */

seq_t a(idx_t n)
{
    if (n == 0)                 /* behaviour below the offset is unspecified */
        return 0;

    idx_t idx = n - 1;          /* zero‑based index of the term   */
    idx_t q   = idx / 3;        /* which 7‑block the term is in   */
    idx_t r   = idx % 3;        /* position inside the block      */

    seq_t res = (seq_t)7 * (seq_t)q;   /* base value 7*q                 */

    /* add the appropriate offset: 0 -> +1, 1 -> +4, 2 -> +6           */
    switch (r) {
        case 0: res += 1; break;
        case 1: res += 4; break;
        case 2: res += 6; break;
    }

    return res;
}
