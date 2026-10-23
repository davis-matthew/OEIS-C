/*
 * A105843  Numbers n such that n^2 = 37*m^2 + 37*m +1.
 * https://oeis.org/A105843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=149, a(3)=295, a(4)=21755, and for n≥5 a(n) =
 *            146*a(n-2) - a(n-4). The recurrence applies separately to the
 *            odd‑indexed and even‑indexed subsequences
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)149;
    if (n == 3) return (seq_t)295;
    if (n == 4) return (seq_t)21755;

    /* Odd indices 1,3,5,7,... use the recurrence a(i)=146*a(i-2)-a(i-4) */
    if (n & 1) {
        seq_t prevprev = (seq_t)1;   /* a(1)  */
        seq_t prev     = (seq_t)295; /* a(3)  */
        for (idx_t i = 5; i <= n; i += 2) {
            seq_t val = 146 * prev - prevprev;
            prevprev = prev;
            prev = val;
        }
        return prev;
    }

    /* Even indices 2,4,6,8,... use the same recurrence */
    seq_t prevprev = (seq_t)149;    /* a(2)  */
    seq_t prev     = (seq_t)21755;  /* a(4)  */
    for (idx_t i = 6; i <= n; i += 2) {
        seq_t val = 146 * prev - prevprev;
        prevprev = prev;
        prev = val;
    }
    return prev;
}
