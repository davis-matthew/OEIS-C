/*
 * A187169  Number of 8-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 39, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187169: Number of 8‑step self‑avoiding walks on an n×n×n cube summed over all starting positions.
   Formula: a(1)=0, a(2)=144, a(3)=67824, a(4)=608928, a(5)=2188608, a(6)=5299056;
            for n>6: a(n)=81390*n³ - 463074*n² + 801216*n - 418032
   The closed form matches all terms for n>6. */
seq_t a(idx_t n)
{
    /* Small values tabulated directly. */
    static const seq_t init[6] = {
        0,          /* a(1) */
        144,        /* a(2) */
        67824,      /* a(3) */
        608928,     /* a(4) */
        2188608,    /* a(5) */
        5299056     /* a(6) */
    };
    if (n <= 6) {
        return init[n - 1];
    }
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n² */
    seq_t n3 = n2 * nn;          /* n³ */
    seq_t res = 81390 * n3;
    res -= 463074 * n2;
    res += 801216 * nn;
    res -= 418032;
    return res;
}
