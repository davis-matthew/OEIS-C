/*
 * A055866  Largest number that has the same number of digits in base 10 and
 *          base n, or 0 if no such number exists.
 * https://oeis.org/A055866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer whose decimal length equals its base-n length,
   or 0 when no such number exists (n = 10). */
seq_t a(idx_t n)
{
    if (n == (idx_t)10) {
        return (seq_t)0;
    }
    seq_t n_seq   = (seq_t)n;
    seq_t best    = (seq_t)0;
    seq_t pow10_prev = (seq_t)1;   /* 10^(d-1) */
    seq_t pow10_curr = (seq_t)10;  /* 10^d       */
    seq_t n_pow_prev = (seq_t)1;   /* n^(d-1)    */
    seq_t n_pow_curr = n_seq;      /* n^d        */

    for (;;) {
        /* Check if the intervals [n^(d-1), n^d-1] and [10^(d-1), 10^d-1] overlap. */
        if (!((n_pow_prev) <= (pow10_curr - (seq_t)1)) ||
            !((pow10_prev) <= (n_pow_curr - (seq_t)1))) {
            break;
        }

        /* Candidate is the smaller of the interval upper bounds. */
        seq_t lim10 = pow10_curr - (seq_t)1;
        seq_t limN  = n_pow_curr - (seq_t)1;
        seq_t cand  = (lim10 <= limN) ? lim10 : limN;
        if (cand > best) {
            best = cand;
        }

        /* Advance to next possible digit count. */
        pow10_prev = pow10_curr;
        pow10_curr = pow10_curr * (seq_t)10;
        n_pow_prev = n_pow_curr;
        n_pow_curr = n_pow_curr * n_seq;
    }
    return best;
}
