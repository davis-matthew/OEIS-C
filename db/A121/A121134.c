/*
 * A121134  Numbers of isomers of unbranched a-4-catapolypentagons - see
 *          Brunvoll reference for precise definition.
 * https://oeis.org/A121134
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function of OEIS A121134:
   a(n) = 6*a(n-1) + 44*a(n-4) + 32*a(n-7) – (8*a(n-2) + 16*a(n-3) + 8*a(n-5) + 48*a(n-6) + N(n))
   where N(0)=0, N(1)=–1, N(2)=4, N(3)=–1, N(4)=–13, N(5)=8, N(6)=20,
   N(7)=–10, N(8)=–18, N(9)=4, N(10)=8, and N(n)=0 for n>10.  */
/* a(2) = 1, a(3) = 2, a(4) = 5, …  */

seq_t a(idx_t n)
{
    if (n < 2)                /* values below the offset are irrelevant */
        return 0;

    /* target is the index of the recurrence value that corresponds to a(n) */
    idx_t target = n - 1;

    /* sliding window of the last 7 computed terms */
    seq_t buf[7] = { 0 };

    /* N(n) coefficients for n = 0 .. 10 */
    static const int N_int[11] = { 0, -1,  4, -1, -13,  8, 20,
                                   -10, -18,  4,  8 };

    for (idx_t i = 0; i <= target; ++i)
    {
        seq_t tmp = 0;
        seq_t sub = 0;

        if (i > 0)      tmp += buf[(i - 1) % 7] * 6;
        if (i > 3)      tmp += buf[(i - 4) % 7] * 44;
        if (i > 6)      tmp += buf[(i - 7) % 7] * 32;

        if (i > 1)      sub += buf[(i - 2) % 7] * 8;
        if (i > 2)      sub += buf[(i - 3) % 7] * 16;
        if (i > 4)      sub += buf[(i - 5) % 7] * 8;
        if (i > 5)      sub += buf[(i - 6) % 7] * 48;

        if (i <= 10) {
            int ni = N_int[i];
            if (ni < 0)
                tmp += (seq_t)(-ni);      /* add |N(n)| when N(n) is negative */
            else
                sub += (seq_t)ni;         /* subtract N(n) when N(n) is positive */
        }

        seq_t val = tmp - sub;          /* tmp ≥ sub for all n */

        if (i == target)  /* the desired term is reached */
            return val;

        buf[i % 7] = val;                /* store the current term for future use */
    }

    return 0; /* should never be reached */
}
