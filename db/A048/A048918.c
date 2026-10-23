/*
 * A048918  9-gonal hexagonal numbers.
 * https://oeis.org/A048918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4162056194*a(n-2) - a(n-4) + 1189158912 (valid for n ≥ 5;
 *            the first four terms are: 1, 325, 5330229625, 1353857339341)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* small sequences directly returned */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)325;
    if (n == 3) return (seq_t)5330229625ULL;
    if (n == 4) return (seq_t)1353857339341ULL;

    /* recurrence: a(n) = 4162056194*a(n-2) - a(n-4) + 1189158912 */
    const seq_t C1 = (seq_t)4162056194ULL;
    const seq_t C2 = (seq_t)1189158912ULL;

    /* a1, a2, a3, a4 */
    seq_t t0 = (seq_t)1;                /* a_{k-4}   */
    seq_t t1 = (seq_t)325;              /* a_{k-3}   */
    seq_t t2 = (seq_t)5330229625ULL;    /* a_{k-2}   */
    seq_t t3 = (seq_t)1353857339341ULL; /* a_{k-1}   */
    seq_t ai = t3;                      /* placeholder */

    for (idx_t k = 5; k <= n; ++k) {
        ai = C1 * t2 - t0 + C2;   /* a_k = 4162056194*a_{k-2} - a_{k-4} + 1189158912 */

        /* shift the window: drop oldest, add new */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = ai;
    }

    return ai;
}
