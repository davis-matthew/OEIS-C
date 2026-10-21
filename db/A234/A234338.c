/*
 * A234338  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to
 *          14.
 * https://oeis.org/A234338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * 11*a(n-1) + 100*a(n-2) - 657*a(n-3) - 3362*a(n-4) +
 *            13855*a(n-5) * + 47579*a(n-6) - 118877*a(n-7) - 254743*a(n-8) +
 *            334913*a(n-9) * + 167982*a(n-10) - 130132*a(n-11) -
 *            15912*a(n-12) + 10008*a(n-13) * (empirical recurrence, valid for
 *            n ≥ 14)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* first 13 terms, indices 1..13 */
    static const seq_t base[13] = {
        (seq_t)604,
        (seq_t)9076,
        (seq_t)120838,
        (seq_t)1709528,
        (seq_t)23776684,
        (seq_t)333289866,
        (seq_t)4661112720,
        (seq_t)65252337120,
        (seq_t)913202183554,
        (seq_t)12781885209276,
        (seq_t)178897637723208,
        (seq_t)2503927583256606,
        (seq_t)35045839285941264
    };

    if (n <= 13)
        return base[n - 1];

    /* sliding window of the last 13 values */
    seq_t prev[13];
    for (int i = 0; i < 13; ++i)
        prev[i] = base[i];

    seq_t next;
    for (idx_t k = 14; k <= n; ++k) {
        if (sizeof(seq_t) <= 8) { /* use 128‑bit intermediate to avoid overflow */
            __int128 pos = 0, neg = 0;
            pos += (__int128)prev[12] * 11;
            pos += (__int128)prev[11] * 100;
            neg += (__int128)prev[10] * 657;
            neg += (__int128)prev[9]  * 3362;
            pos += (__int128)prev[8]  * 13855;
            pos += (__int128)prev[7]  * 47579;
            neg += (__int128)prev[6]  * 118877;
            neg += (__int128)prev[5]  * 254743;
            pos += (__int128)prev[4]  * 334913;
            pos += (__int128)prev[3]  * 167982;
            neg += (__int128)prev[2]  * 130132;
            neg += (__int128)prev[1]  * 15912;
            pos += (__int128)prev[0]  * 10008;
            next = (seq_t)(pos - neg);
        } else { /* seq_t has sufficient width (≥128 bits) */
            seq_t pos = 0, neg = 0;
            pos += prev[12] * 11;
            pos += prev[11] * 100;
            neg += prev[10] * 657;
            neg += prev[9]  * 3362;
            pos += prev[8]  * 13855;
            pos += prev[7]  * 47579;
            neg += prev[6]  * 118877;
            neg += prev[5]  * 254743;
            pos += prev[4]  * 334913;
            pos += prev[3]  * 167982;
            neg += prev[2]  * 130132;
            neg += prev[1]  * 15912;
            pos += prev[0]  * 10008;
            next = pos - neg;
        }

        /* shift window and insert new term */
        for (int i = 0; i < 12; ++i)
            prev[i] = prev[i + 1];
        prev[12] = next;
    }

    return prev[12];
}
