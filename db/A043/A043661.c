/*
 * A043661  Numbers whose base-13 representation has exactly 6 runs.
 * https://oeis.org/A043661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-13 representation has exactly 6 runs
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   capped_comb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/capped_comb.h"

/* precomputed powers of 12 up to 12^6 */
static const uint64_t pow12[7] = {1ULL, 12ULL, 144ULL, 1728ULL, 20736ULL, 248832ULL, 2985984ULL};

/* total numbers with exactly 6 runs for all lengths <= L: 12^6 * C(L,6) */
static uint64_t total_len(uint64_t L)
{
    uint64_t comb = capped_comb(L, 6);
    __int128 t = (__int128)pow12[6] * comb;
    if (t > (uint64_t)-1) t = (uint64_t)-1;
    return (uint64_t)t;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* out of range, return 0 as a placeholder */

    /* find the length L (number of base-13 digits) for the nth element */
    uint64_t L = 6;
    while (total_len(L) < n) ++L;
    uint64_t prev_total = (L == 6) ? 0 : total_len(L - 1);
    uint64_t offset = n - 1 - prev_total;    /* 0‑based offset inside length L */

    /* arrays for run lengths and actual digits */
    uint64_t run_len[6];
    uint8_t  run_digit[6];
    uint64_t rem_len = L;
    int      rem_runs = 6;
    uint8_t  prev_digit = 0;          /* placeholder, will be set after first digit */

    for (int run = 0; run < 6; ++run) {
        /* choose run length l_i */
        uint64_t l_choice = 1;
        for (uint64_t l = 1; l <= rem_len - (rem_runs - 1); ++l) {
            uint64_t comb = capped_comb(rem_len - l - 1, rem_runs - 2);
            __int128 count_l = (__int128)pow12[rem_runs] * comb;
            uint64_t count_l64 = (count_l > (uint64_t)-1) ? (uint64_t)-1 : (uint64_t)count_l;
            if (offset < count_l64) { l_choice = l; break; }
            offset -= count_l64;
        }
        run_len[run] = l_choice;

        /* choose digit for this run */
        uint64_t count_digit_each;
        if (rem_runs == 1) {          /* last run, only one completion per digit */
            count_digit_each = 1;
        } else {
            uint64_t comb = capped_comb(rem_len - l_choice - 1, rem_runs - 2);
            __int128 count = (__int128)pow12[rem_runs - 1] * comb;
            count_digit_each = (count > (uint64_t)-1) ? (uint64_t)-1 : (uint64_t)count;
        }

        uint64_t d_index = offset / count_digit_each;
        offset %= count_digit_each;

        uint8_t digit_val;
        if (run == 0) {
            digit_val = (uint8_t)(d_index + 1);     /* first run: digits 1..12 */
        } else {
            /* exclude previous digit */
            digit_val = (uint8_t)(d_index < prev_digit ? d_index : d_index + 1);
        }
        run_digit[run] = digit_val;
        prev_digit = digit_val;

        /* prepare for next run */
        rem_len -= l_choice;
        --rem_runs;
    }

    /* build the number from the runs */
    seq_t val = 0;
    for (int run = 0; run < 6; ++run) {
        for (uint64_t k = 0; k < run_len[run]; ++k) {
            val = val * 13 + (seq_t)run_digit[run];
        }
    }
    return val;
}
