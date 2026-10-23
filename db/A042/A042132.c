/*
 * A042132  Numerators of continued fraction convergents to sqrt(591).
 * https://oeis.org/A042132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042132: numerators of continued‑fraction convergents to sqrt(591).
   Recurrence: a(n) = 331352*a(n-10) - a(n-20).
   Initial terms (n = 0 … 19):
   24, 73, 316, 389, 705, 5324, 6029, 11353, 51441, 165676,
   8003889, 24177343, 104713261, 128890604, 233603865, 1764117659,
   1997721524, 3761839183, 17045078256, 54897073951.
*/

seq_t a(idx_t n)
{
    /* constants */
    const unsigned long long D  = 591ULL;
    const unsigned long long a0 = 24ULL;

    /* convergents recurrence */
    seq_t p_minus1 = (seq_t)1;      /* p_{-1} */
    seq_t p_prev   = (seq_t)a0;     /* p_0 = a0 */

    if (n == 0)
        return p_prev;

    /* continued fraction state for sqrt(591) */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a_k = a0;    /* current partial quotient (starts with a0) */

    for (idx_t i = 1; i <= n; ++i) {
        unsigned long long next_m = d * a_k - m;
        unsigned long long next_d = (D - next_m * next_m) / d;
        unsigned long long next_a = (a0 + next_m) / next_d;

        /* p_i = next_a * p_prev + p_minus1 */
        seq_t mul       = p_prev * (seq_t)next_a;
        seq_t p_current = mul + p_minus1;

        /* advance state */
        p_minus1 = p_prev;
        p_prev   = p_current;
        m = next_m;
        d = next_d;
        a_k = next_a;
    }

    return p_prev;
}
