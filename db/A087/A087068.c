/*
 * A087068  Sum{Floor(n*r^k): r=2/3, k>=0}.
 * https://oeis.org/A087068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} ⌊ n * (2/3)^k ⌋
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A087068. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t num = (seq_t)n;   /* n * 2^k          */
    seq_t den = 1;          /* 3^k              */

    for (;;)
    {
        seq_t term = num / den;      /* floor of n * (2/3)^k */
        if (term == 0)
            break;                  /* further terms will all be 0 */
        sum += term;

        /* Advance to the next k: multiply numerator by 2 and denominator by 3
           using a 128‑bit intermediate to avoid overflow. */
        num = (seq_t)((unsigned __int128)num * 2);
        den = (seq_t)((unsigned __int128)den * 3);
    }

    return sum;
}
