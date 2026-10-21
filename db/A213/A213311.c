/*
 * A213311  Numbers with exactly 4 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 633, u32 n <= 653, u64 n <= 653,
 *            u128 n <= 653, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A213311: numbers with exactly 4 nonprime substrings.
 *  The function computes the sequence by brute‑force enumeration
 *  up to a fixed upper bound (400 000), which is sufficient for all
 *  known terms (the 653rd term is 373 379).  All values are stored in
 *  a static array for O(1) subsequent access.  Leading zeros are
 *  treated as nonprime substrings, as required.  The search stops
 *  once the table is filled; if n exceeds the number of
 *  available terms the function returns 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*--- Prime table up to 1 000 000 ------------------------------------------------*/

static bool prime_table[1000001];
static bool prime_table_built = false;

static void build_prime_table(void)
{
    if (prime_table_built) return;
    for (int i = 0; i <= 1000000; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;
    for (int i = 2; i * i <= 1000000; ++i) {
        if (prime_table[i]) {
            for (int j = i * i; j <= 1000000; j += i) prime_table[j] = false;
        }
    }
    prime_table_built = true;
}

/*--- Test whether a number has exactly 4 nonprime substrings -----------------*/

static bool has_four_nonprime_substrings(uint32_t num)
{
    /* Extract decimal digits into an array (least‑significant first) */
    uint8_t digits[10];
    int len = 0;
    if (num == 0) { digits[0] = 0; len = 1; }
    while (num > 0) {
        digits[len++] = num % 10;
        num /= 10;
    }
    /* Reverse to obtain the forward order of digits */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    int count = 0;
    for (int i = 0; i < len; ++i) {
        uint32_t val = 0;
        uint8_t first = digits[i];
        for (int j = i; j < len; ++j) {
            val = val * 10 + digits[j];
            if (first == 0 || !prime_table[val]) {
                ++count;
                if (count > 4) return false;
            }
        }
    }
    return count == 4;
}

/*--- Sequence storage ---------------------------------------------------------*/

static seq_t seq_values[700];     /* 700 > 653 (max number of terms) */
static size_t seq_len = 0;
static bool seq_built = false;

static void build_sequence(void)
{
    if (seq_built) return;
    build_prime_table();
    const uint32_t MAX_CANDIDATE = 400000;  /* > 373379 */
    for (uint32_t cand = 1; cand <= MAX_CANDIDATE; ++cand) {
        if (has_four_nonprime_substrings(cand)) {
            if (seq_len < 700) seq_values[seq_len++] = (seq_t)cand;
        }
    }
    seq_built = true;
}

/*--- Public function ----------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0 */
    build_sequence();
    if ((idx_t)(n - 1) < (idx_t)seq_len) return seq_values[n - 1];
    return 0;                       /* n beyond sequence bounds */
}
