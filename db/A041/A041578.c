/*
 * A041578  Numerators of continued fraction convergents to sqrt(307).
 * https://oeis.org/A041578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p(-2)=0, p(-1)=1.
   a_k are continued‑fraction coefficients of √307:
   a_0 = 17; for k≥1, a_k cycles through
   1,1,11,5,1,3,17,3,1,5,11,1,1,34 (period 14).
   Then p_k = a_k·p_{k-1} + p_{k-2}.
   The function returns p_n. */
seq_t a(idx_t n)
{
    const idx_t period = 14;
    static const unsigned long long period_vals[14] =
        {1ULL,1ULL,11ULL,5ULL,1ULL,3ULL,17ULL,3ULL,1ULL,5ULL,11ULL,1ULL,1ULL,34ULL};

    seq_t prevprev = 0; /* p_{-2} */
    seq_t prev     = 1; /* p_{-1} */
    seq_t curr     = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (k == 0) ? (seq_t)17
                           : (seq_t)period_vals[(k - 1) % period];
        curr = ak * prev + prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}
