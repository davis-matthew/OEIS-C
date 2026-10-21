/*
 * A077400  Triangular numbers that are 7 times triangular numbers.
 * https://oeis.org/A077400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A077400):
   a(0)=0, a(1)=21, a(2)=105, a(3)=5460
   For n ≥ 4:  a(n) = 254*a(n-2) - a(n-4) + 126
   This recurrence generates exactly the sequence. */

seq_t a(idx_t n)
{
    /* Handle the first four explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 21;
    if (n == 2) return 105;
    if (n == 3) return 5460;

    /* v[0] … v[3] hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t v[4];
    v[0] = 0;      /* a(0)   */
    v[1] = 21;     /* a(1)   */
    v[2] = 105;    /* a(2)   */
    v[3] = 5460;   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 254 * v[2] - v[0] + 126;  /* a(i) = 254*a(i-2) - a(i-4) + 126 */
        /* Shift: drop a(i-4), add a(i) */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = next;
    }

    return v[3]; /* a(n) is now in v[3] */
}
