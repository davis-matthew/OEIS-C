/*
 * A031942  Numbers with no consecutive equal base 4 digits.
 * https://oeis.org/A031942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑4 representation
 *            contains no consecutive equal digits. Computed by combinatorial
 *            mapping. The algorithm works in O(log_3 n) time and uses only
 *            integer operations
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the number of base‑4 digits k such that the cumulative count of
       numbers with up to k digits >= n.  Count of k‑digit numbers is 3^k.  */
    idx_t total = 0;          /* cumulative count up to current k */
    idx_t power = 1;          /* 3^(k-1) before incrementing k */
    idx_t prev_total = 0;     /* total before current k */
    idx_t k = 0;

    do {
        ++k;
        power *= 3;          /* now power = 3^k */
        prev_total = total;
        total += power;      /* add 3^k */
    } while (total < n);

    /* Offset within the k‑digit block (0‑based). */
    idx_t offset = n - prev_total - 1;

    /* Precompute the most significant divisor for extracting base‑3 digits
       of the offset.  divisor = 3^(k‑1).  */
    idx_t divisor = power / 3;

    /* Mapping of previous base‑4 digit to the next valid digit,
       based on the set {0,1,2,3} \ {previous}.  The table is sorted
       ascending. */
    static const seq_t MAP[4][3] = {
        {1, 2, 3},  /* previous 0 -> {1,2,3} */
        {0, 2, 3},  /* previous 1 -> {0,2,3} */
        {0, 1, 3},  /* previous 2 -> {0,1,3} */
        {0, 1, 2}   /* previous 3 -> {0,1,2} */
    };

    seq_t value = 0;
    seq_t prev   = 0;     /* previous digit (0–3) */

    for (idx_t pos = 0; pos < k; ++pos) {
        /* Extract the next digit in base‑3 representation of offset. */
        idx_t d = offset / divisor;
        offset %= divisor;
        if (pos == 0) {
            /* First digit cannot be 0 in base‑4 representation. */
            value = 1 + (seq_t)d;   /* digits 1,2,3 */
            prev  = value;
        } else {
            /* Use the mapping table for the next digit. */
            value = value * 4 + MAP[(unsigned int)prev][(unsigned int)d];
            prev  = value % 4;
        }
        if (divisor > 1) divisor /= 3;   /* move to next lower place */
    }

    return value;
}
