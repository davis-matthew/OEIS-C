/*
 * A043449  Numbers having one 7 in base 8.
 * https://oeis.org/A043449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑8 representation contains
 *            exactly one digit 7. This implementation computes a(n) by
 *            combinatorial indexing
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* n ≥ 1 in the OEIS offset */

    seq_t n_seq = (seq_t)n;
    seq_t cumulative = 0;          /* count of numbers with length < d */
    seq_t pow = 1;                  /* 7^(d-1), distance from 8‑digit boundary */
    idx_t d = 1;                    /* current digit length */

    /* find the digit length d of the n‑th number */
    for (;;) {
        seq_t cnt;
        if (d == 1) {
            cnt = 1;               /* only 7 */
        } else {
            seq_t pow_d2 = pow / 7;         /* 7^(d-2) */
            cnt = pow + (seq_t)(d - 1) * 6 * pow_d2;
        }
        if (cumulative + cnt >= n_seq) break;
        cumulative += cnt;
        pow *= (seq_t)7;              /* next power: 7^d */
        ++d;
    }

    /* d is now the number of base‑8 digits of a(n) */
    if (d == 1) return (seq_t)7;     /* trivial case */

    seq_t pow_d2 = pow / 7;          /* 7^(d-2) */
    seq_t count_leading_7 = pow;     /* 7^(d-1) numbers with leading 7   */
    seq_t per_leading_cnt = (seq_t)(d - 1) * pow_d2;  /* suffix count per leading digit */
    seq_t total_leading_non7 = 6 * per_leading_cnt;    /* 6 leading digits (1..6) */

    /* offset of a(n) within the d‑digit block (0‑based) */
    seq_t offset_seq = n_seq - cumulative - 1;
    idx_t offset = (idx_t)offset_seq;            /* fits into idx_t */

    int digits[64];   /* buffer for the d base‑8 digits (d ≤ 64 for any n) */

    if (offset < total_leading_non7) {
        /* case: leading digit is 1..6 */
        seq_t offset_seq2 = offset_seq;
        idx_t idxL = (idx_t)(offset_seq2 / per_leading_cnt);
        seq_t suffix_off_seq = offset_seq2 % per_leading_cnt;
        int leading = (int)(idxL + 1);  /* 1..6 */
        digits[0] = leading;

        int k = d - 1;                     /* length of the suffix */
        /* position of the digit 7 inside the suffix (0 = least‑significant) */
        idx_t pos_low = (idx_t)(suffix_off_seq / pow_d2);
        seq_t rem_rem_seq = suffix_off_seq % pow_d2;
        int pos_from_left = (int)((k - 1) - pos_low); /* index from most‑significant */

        /* build the suffix */
        int suffix[k];
        seq_t rem = rem_rem_seq;
        for (int i = k - 1; i >= 0; --i) {
            if (i == pos_from_left) {
                suffix[i] = 7;
            } else {
                suffix[i] = (int)(rem % 7);
                rem /= 7;
            }
        }
        for (int i = 0; i < k; ++i) digits[1 + i] = suffix[i];
    } else {
        /* case: leading digit is 7 */
        seq_t suffix_off_seq = offset_seq - total_leading_non7;
        digits[0] = 7;
        int k = d - 1;                     /* length of the suffix */
        seq_t rem = suffix_off_seq;
        for (int i = k - 1; i >= 0; --i) {
            digits[1 + i] = (int)(rem % 7);
            rem /= 7;
        }
    }

    /* convert the digit array back to an integer */
    seq_t value = 0;
    for (int i = 0; i < d; ++i) {
        value = value * 8 + (seq_t)digits[i];
    }
    return value;
}
