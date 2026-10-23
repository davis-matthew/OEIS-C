/*
 * A043484  Numbers having four 7's in base 9.
 * https://oeis.org/A043484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having four 7's in base 9
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow8_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow8_shift.h"

static inline uint64_t comb(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (unsigned i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = n;          /* 1‑based index in the whole sequence            */
    int    L;             /* length of the base‑9 representation            */

    /* find the length L such that the n-th number lies in that length   */
    for (L = 4; ; ++L) {
        uint64_t countL = 0;

        /* numbers with the leading digit equal to 7                       */
        uint64_t c_with = comb(L - 1, 3);
        countL += c_with * pow8_shift(L - 4);

        /* numbers with the leading digit different from 7                */
        if (L - 1 >= 4) {
            uint64_t c_without = comb(L - 1, 4);
            countL += c_without * 7ULL * pow8_shift(L - 5);
        }

        if (m > countL) {
            m -= countL;   /* skip this block                                  */
        } else {
            break;          /* m is now 1‑based index within length L           */
        }
    }

    /* unranking within numbers of length L                               */
    int digits[20];        /* L will not exceed 20 for any reasonable n         */
    int sevens_used = 0;   /* number of 7's chosen so far                       */

    for (int pos = 0; pos < L; ++pos) {
        int start_digit = (pos == 0) ? 1 : 0;  /* no leading zero                */

        for (int d = start_digit; d <= 8; ++d) {
            int new_sevens = sevens_used + (d == 7 ? 1 : 0);
            int remaining_positions = L - (pos + 1);
            int remaining_needed   = 4 - new_sevens;

            if (remaining_needed < 0 || remaining_needed > remaining_positions)
                continue;   /* impossible assignment for this digit           */

            uint64_t ways = comb(remaining_positions, remaining_needed);
            uint64_t total_possible =
                ways * pow8_shift(remaining_positions - remaining_needed);

            if (m > total_possible) {
                m -= total_possible;
                continue;   /* try next digit                               */
            } else {
                digits[pos] = d;
                sevens_used = new_sevens;
                break;      /* move to next position                         */
            }
        }
    }

    /* convert the base‑9 digits to a decimal seq_t value                 */
    seq_t result = 0;
    for (int i = 0; i < L; ++i) {
        result = result * 9 + (seq_t)digits[i];
    }
    return result;
}
