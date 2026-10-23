/*
 * A030143  Even numbers in which parity of digits alternates.
 * https://oeis.org/A030143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th even number whose decimal digits alternate
 *            parity
 * Range:     u8 n <= 43, u16 n <= 2330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined – return 0 as a safe fallback. */
    if (n == 0) return 0;

    /* Find the length L of the n‑th term and the cumulative count of all
       shorter lengths.  powPrev holds 5^(L-1). */
    idx_t cumulative = 0;   /* terms counted so far */
    idx_t L = 1;            /* current length */
    idx_t powPrev = 1;      /* 5^(L-1) */

    while (1) {
        idx_t group;
        if (L == 1) {
            group = 5;                /* 0,2,4,6,8 */
        } else if (L % 2 == 0) {
            group = 5 * powPrev;      /* leading odd (5 choices) */
        } else {
            group = 4 * powPrev;      /* leading even non‑zero (4 choices) */
        }

        if (cumulative + group >= n) {
            /* n is within this length group. */
            break;
        }

        cumulative += group;
        ++L;
        powPrev *= 5;                /* next 5^(L-1) */
    }

    /* 0‑based index inside the current length group. */
    idx_t offset = n - cumulative - 1;

    /* Single‑digit case -------------------------------------------------- */
    if (L == 1) {
        /* 0,2,4,6,8 → 2 * offset */
        return (seq_t)(offset * 2);
    }

    /* Multidigit case ----------------------------------------------------- */
    idx_t powLast = powPrev;                /* 5^(L-1) */
    idx_t leading_idx = offset / powLast;   /* index of the leading digit */
    idx_t rem = offset % powLast;           /* remainder for the remaining digits */

    /* Determine leading digit based on length parity. */
    seq_t leading_digit;
    if (L % 2 == 0) {                      /* even length → leading odd */
        leading_digit = (seq_t)(1 + 2 * leading_idx);  /* 1,3,5,7,9 */
    } else {                                /* odd length → leading even non‑zero */
        leading_digit = (seq_t)(2 + 2 * leading_idx);  /* 2,4,6,8 */
    }

    seq_t result = leading_digit;

    /* Build the remaining L-1 digits from most significant to least. */
    idx_t power = powLast / 5;              /* 5^(L-2) – weight of the 2nd digit */
    for (idx_t j = 2; j <= L; ++j) {
        idx_t digit_idx = rem / power;
        rem %= power;

        seq_t digit;
        if (((L - j) & 1) == 0) {          /* even parity position */
            digit = (seq_t)(0 + 2 * digit_idx);  /* 0,2,4,6,8 */
        } else {                           /* odd parity position */
            digit = (seq_t)(1 + 2 * digit_idx);  /* 1,3,5,7,9 */
        }

        result = result * 10 + digit;

        if (j < L) {
            power /= 5;                    /* move to the next lower power of 5 */
        }
    }

    return result;
}
