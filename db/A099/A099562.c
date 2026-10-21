/*
 * A099562  Sum C(n-4k,k-1), k=0..floor(n/5).
 * https://oeis.org/A099562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{floor(n/5)} C(n-4k , k-1) Recurrence: a(n) =
 *            a(n-1) + 2*a(n-5) - a(n-6) - a(n-10), with
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1
 * Range:     u8 n <= 26, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the value iteratively, using a circular
   buffer of the last 11 terms. No external state is required. */

seq_t a(idx_t n)
{
    /* Base values for n = 0..5 */
    if (n <= 5) {
        switch ((int)n) {
        case 0: case 1: case 2: case 3: case 4: return 0;
        case 5: return 1;
        default: return 0; /* never reached */
        }
    }

    /* A circular buffer holding the last 11 computed values.
       Index i corresponds to term a(i). */
    seq_t buf[11];
    /* Initialise known base values */
    buf[0] = 0;  /* a(0)   */
    buf[1] = 0;  /* a(1)   */
    buf[2] = 0;  /* a(2)   */
    buf[3] = 0;  /* a(3)   */
    buf[4] = 0;  /* a(4)   */
    buf[5] = 1;  /* a(5)   */
    buf[6] = 0;  /* will be overwritten */
    buf[7] = 0;
    buf[8] = 0;
    buf[9] = 0;
    buf[10] = 0;

    for (idx_t i = 6; i <= n; ++i) {
        idx_t idx = i % 11;
        seq_t v1 = buf[(i - 1) % 11];
        seq_t v5 = buf[(i - 5) % 11];
        seq_t v6 = buf[(i - 6) % 11];
        seq_t v10 = (i >= 10) ? buf[(i - 10) % 11] : (seq_t)0;

        /* Recurrence: a(i) = a(i-1) + 2*a(i-5) - a(i-6) - a(i-10) */
        seq_t val = v1 + (seq_t)2 * v5 - v6 - v10;
        buf[idx] = val;
    }

    return buf[n % 11];
}
