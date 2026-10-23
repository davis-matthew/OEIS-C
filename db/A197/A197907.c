/*
 * A197907  Ceiling((n+1/n)^8).
 * https://oeis.org/A197907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/n)^8) = ceil(((n^2 + 1)^8) / n^8)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 255, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 7 this equals n^8 + 8n^6 + 28n^4 + 56n^2 + 71. */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert the idx_t to seq_t first */

    if (n <= 7) {
        /* Exact rational computation that is safe for n ≤ 7. */
        seq_t base  = nseq * nseq + (seq_t)1;      /* n^2 + 1          */
        seq_t num   = (seq_t)1;                    /* (n^2 + 1)^8      */
        for (int i = 0; i < 8; ++i) num *= base;

        seq_t den   = (seq_t)1;                    /* n^8              */
        for (int i = 0; i < 8; ++i) den *= nseq;

        return (num + den - (seq_t)1) / den;       /* ceil division   */
    } else {
        /* Polynomial form valid for n > 7 (ceil gives +1). */
        seq_t n2 = nseq * nseq;        /* n^2      */
        seq_t n4 = n2 * n2;            /* n^4      */
        seq_t n6 = n4 * n2;            /* n^6      */
        seq_t n8 = n4 * n4;            /* n^8      */

        return n8
             + (seq_t)8 * n6
             + (seq_t)28 * n4
             + (seq_t)56 * n2
             + (seq_t)71;              /* +71 == +70 + 1 (ceil) */
    }
}
