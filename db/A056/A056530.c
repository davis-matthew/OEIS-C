/*
 * A056530  Sequence remaining after third round of Flavius Josephus sieve;
 *          remove every fourth term of A047241.
 * https://oeis.org/A056530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A056530: a(n) = a(n‑1) + a(n‑3) – a(n‑4)  (n > 4)
 * with initial values: a(1)=1, a(2)=3, a(3)=7, a(4)=13.
 */
seq_t a(idx_t n)
{
    /* Handle first four terms directly */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)3;
            case 3: return (seq_t)7;
            case 4: return (seq_t)13;
            default: return (seq_t)0;   /* should not happen; offset is 1 */
        }
    }

    /* Use a rolling window of the last four values */
    seq_t a1 = (seq_t)1;   /* a(n‑4) */
    seq_t a2 = (seq_t)3;   /* a(n‑3) */
    seq_t a3 = (seq_t)7;   /* a(n‑2) */
    seq_t a4 = (seq_t)13;  /* a(n‑1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a4 + a2 - a1;   /* a(i) = a(i-1)+a(i-3)-a(i-4) */
        /* Shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
