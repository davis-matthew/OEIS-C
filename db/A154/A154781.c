/*
 * A154781  Sum of all numbers < n that appear as substring of n, written in
 *          decimal system.
 * https://oeis.org/A154781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of all distinct integer values of substrings of n's
 *            decimal representation that are less than n
 * Range:     u8 n <= 276, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t num = (seq_t)n;          /* keep the value as seq_t for comparisons */

    /* Extract decimal digits of n into array 'dig' in the normal order (most
       significant digit first).  Maximum digits for idx_t is safely below 32. */
    int dig[32];
    int len = 0;
    if (n == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        int rev[32];
        while (tmp > 0) {
            rev[len++] = (int)(tmp % 10);
            tmp /= 10;
        }
        for (int i = 0; i < len; i++)
            dig[i] = rev[len - 1 - i];
    }

    /* Store the distinct values that we have already counted.  The number of
       different substrings is bounded by len*(len+1)/2, which is at most 15
       for n <= 10000. */
    seq_t seen[20];
    int seen_cnt = 0;

    for (int i = 0; i < len; i++) {
        seq_t val = 0;
        for (int j = i; j < len; j++) {
            val = val * 10 + (seq_t)dig[j];
            if (val >= num)
                break;               /* further digits only make it larger */

            /* Check if we have already added this value. */
            int dup = 0;
            for (int k = 0; k < seen_cnt; k++)
                if (seen[k] == val) { dup = 1; break; }

            if (!dup) {
                seen[seen_cnt++] = val;
                sum += val;
            }
        }
    }
    return sum;
}
