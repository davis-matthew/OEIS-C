/*
 * A180961  Numbers such that the American English name of the number has four
 *          syllables.
 * https://oeis.org/A180961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, word
 * Formula:   a(n) is the nth integer whose American English name has exactly
 *            four syllables (computed by brute‑force syllable counting)
 * Range:     u8 n <= 35, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/append_candidate.h"

static const int unit_syll[10]  = {0,1,1,1,1,1,1,2,1,1}; /* 0–9 */
static const int teen_syll[10]  = {1,3,1,2,2,2,2,3,2,2};   /* 10–19 */
static const int tens_syll[10]  = {0,0,2,2,2,2,2,3,2,2};   /* 20–90 */
static const int groupSyll[11]  = {2,2,3,3,3,4,4,4,4,4,4}; /* thousand, million, … */

/* Count syllables in the American English name of n. */
static int syllable_count(idx_t n)
{
    int total = 0;
    int group_idx = 0;
    while (n > 0) {
        int chunk = (int)(n % 1000);
        n /= 1000;
        if (chunk == 0) {
            group_idx++;
            continue;
        }
        int hundreds = chunk / 100;
        int rem = chunk % 100;
        if (hundreds > 0) {
            total += unit_syll[hundreds];  /* "one" … "nine" */
            total += 2;                    /* "hundred" */
        }
        if (rem >= 10 && rem <= 19) {
            total += teen_syll[rem - 10];
        } else {
            int tens = rem / 10;
            int ones = rem % 10;
            if (tens > 0) {
                total += tens_syll[tens];
                if (ones > 0) total += unit_syll[ones];
            } else if (ones > 0) {
                total += unit_syll[ones];
            }
        }
        if (group_idx > 0) {
            total += groupSyll[group_idx - 1];  /* thousand, million, … */
        }
        group_idx++;
    }
    return total;
}

/* Return the n‑th term of the sequence.  Memoises found terms for efficiency. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_start = 1;  /* next number to test */

    while (cache_len < (size_t)n) {
        if (syllable_count(next_start) == 4) {
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)next_start);
        }
        next_start++;
    }
    return cache[n - 1];
}
