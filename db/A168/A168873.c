/*
 * A168873  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..20) are given explicitly. For n≥21 a(n) = 2 * Σ_{i=1}^{20}
 *            a(n-i) – 3 * a(n-21), except at n=21 the right‑hand side equals
 *            1, so a(21)=2*Σ_{i=1}^{20}a(i)–2. This follows from the rational
 *            generating function a(t)=P(t)/Q(t) with Q(t)=3t^21–2t^20–⋯–2t+1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial 21 terms a(0)..a(20) */
    static const seq_t init[21] = {
        1ULL, 4ULL, 12ULL, 36ULL, 108ULL, 324ULL, 972ULL, 2916ULL, 8748ULL,
        26244ULL, 78732ULL, 236196ULL, 708588ULL, 2125764ULL, 6377292ULL,
        19131876ULL, 57395628ULL, 172186884ULL, 516560652ULL, 1549681956ULL,
        4649045868ULL
    };

    if (n <= 20) return init[n];

    /* buffer holds the most recent 21 terms: a_{k-20} … a_k */
    seq_t buf[21];
    for (idx_t i = 0; i < 21; ++i) buf[i] = init[i];

    /* compute a(21) specially (RHS of recurrence is 1) */
    seq_t sum = 0;
    for (idx_t i = 1; i < 21; ++i) sum += buf[i];          /* sum a(1)…a(20) */
    seq_t a21 = 2 * sum - 2;                               /* a(21) = 2*sum – 2 */

    if (n == 21) return a21;                              /* early exit */

    /* shift window: discard a(0), keep a(1)…a(20) and append a21 */
    for (idx_t i = 0; i < 20; ++i) buf[i] = buf[i + 1];
    buf[20] = a21;                                         /* now buf holds a(1)…a(21) */

    /* iterate from n = 22 up to the requested index */
    for (idx_t k = 22; k <= n; ++k) {
        sum = 0;
        for (idx_t i = 1; i < 21; ++i) sum += buf[i];      /* sum a(k-1)…a(k-20) */
        seq_t next = 2 * sum - 3 * buf[0];                /* recurrence: a(k) = 2*sum – 3*a(k-21) */

        /* slide window: discard oldest, append next */
        for (idx_t i = 0; i < 20; ++i) buf[i] = buf[i + 1];
        buf[20] = next;                                   /* buf now holds a(k-20)…a(k) */
    }

    return buf[20];                                        /* a(n) */
}
