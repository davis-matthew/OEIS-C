/*
 * A183646  Number of (n+1) X 4 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 15*a(n-1) - 85*a(n-2) + 225*a(n-3) - 274*a(n-4) + 120*a(n-5)
 * Initial values: a(1)=1333, a(2)=3445, a(3)=10213, a(4)=33325, a(5)=116653
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1333;
    if (n == 2) return (seq_t)3445;
    if (n == 3) return (seq_t)10213;
    if (n == 4) return (seq_t)33325;
    if (n == 5) return (seq_t)116653;

    seq_t a1 = (seq_t)1333;   /* a(1) */
    seq_t a2 = (seq_t)3445;   /* a(2) */
    seq_t a3 = (seq_t)10213;  /* a(3) */
    seq_t a4 = (seq_t)33325;  /* a(4) */
    seq_t a5 = (seq_t)116653; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = ((seq_t)15) * a5
                   - ((seq_t)85) * a4
                   + ((seq_t)225) * a3
                   - ((seq_t)274) * a2
                   + ((seq_t)120) * a1;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
