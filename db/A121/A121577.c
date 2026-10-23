/*
 * A121577  Times in hours, minutes and seconds (to the nearest second) at
 *          which the hour and minute hands of an analog clock, if
 *          interchanged, continue to indicate some other albeit accurate
 *          times, over a complete 12-hour sweep for the slower hand. Leading
 *          zeros omitted.
 * https://oeis.org/A121577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = round(43200*n/143) formatted as doubly‑spaced sexagesimal
 *            (no leading zero for the hour, minutes and seconds are
 *            zero‑padded to two digits)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the rounded number of seconds: round(43200*n/143) */
    seq_t num = (seq_t)n * (seq_t)43200u;          /* 43200 * n   */
    seq_t q   = num / (seq_t)143u;                /* integer part */
    seq_t r   = num % (seq_t)143u;                /* remainder   */
    if ((r << 1) >= (seq_t)143u)                  /* round up if r*2 >= 143  */
        q += (seq_t)1u;
    seq_t sec = q;                                /* total seconds  */

    /* Convert seconds to hours, minutes, seconds. */
    seq_t hr   = sec / (seq_t)3600u;
    seq_t rem  = sec % (seq_t)3600u;
    seq_t min  = rem / (seq_t)60u;
    seq_t sec2 = rem % (seq_t)60u;

    /* Concatenate into the required decimal format: HMMSS. */
    seq_t res = hr;
    res = res * (seq_t)100u + min;   /* HHMM (minutes zero‑padded) */
    res = res * (seq_t)100u + sec2;  /* HHMMSS (seconds zero‑padded) */

    return res;
}
