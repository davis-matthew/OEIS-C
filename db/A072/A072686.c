/*
 * A072686  Prime numbers whose (British) English names have a prime number of
 *          letters.
 * https://oeis.org/A072686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   ensure_capacity, is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generates primes in ascending order and keeps those whose
 * British English names have a prime number of letters. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

/* Letter counts for numbers 1‑19 */
static const uint8_t unit_len[20] = {
    0, /*0*/
    3, /*ONE*/
    3, /*TWO*/
    5, /*THREE*/
    4, /*FOUR*/
    4, /*FIVE*/
    3, /*SIX*/
    5, /*SEVEN*/
    5, /*EIGHT*/
    4, /*NINE*/
    3, /*TEN*/
    6, /*ELEVEN*/
    6, /*TWELVE*/
    8, /*THIRTEEN*/
    7, /*FOURTEEN*/
    7, /*FIFTEEN*/
    7, /*SIXTEEN*/
    9, /*SEVENTEEN*/
    8, /*EIGHTEEN*/
    8  /*NINETEEN*/
};

/* Letter counts for tens */
static const uint8_t tens_len[10] = {
    0, 0, /*0,1*/
    6, /*TWENTY*/
    6, /*THIRTY*/
    5, /*FORTY*/
    5, /*FIFTY*/
    5, /*SIXTY*/
    7, /*SEVENTY*/
    6, /*EIGHTY*/
    6  /*NINETY*/
};

/* Lengths of fixed words */
static const uint32_t len_and     = 3; /* AND */
static const uint32_t len_hundred = 7; /* HUNDRED */
static const uint32_t len_thousand= 8; /* THOUSAND */
static const uint32_t len_million = 7; /* MILLION */
static const uint32_t len_billion = 7; /* BILLION */

/* Letter count of a 1‑999 group */
static inline uint64_t group_letter_count(uint64_t g)
{
    uint64_t l = 0U;
    if (g == 0) return 0U;
    unsigned h = (unsigned)(g / 100);
    unsigned rem = (unsigned)(g % 100);
    if (h > 0) {
        l += unit_len[h];
        l += len_hundred;
        if (rem > 0) l += len_and;
    }
    if (rem > 0) {
        if (rem < 20) {
            l += unit_len[rem];
        } else {
            unsigned t = rem / 10;        /* 2‑9 */
            unsigned u = rem % 10;        /* 0‑9 */
            l += tens_len[t];
            if (u > 0) l += unit_len[u];
        }
    }
    return l;
}

/* Letter count of the British English name of n (n>0) */
static inline uint64_t number_letter_count(uint64_t n)
{
    /* Split into groups of 3 digits: units, thousands, millions, ... */
    uint64_t groups[10];
    int grp_cnt = 0;
    while (n > 0) {
        groups[grp_cnt++] = n % 1000;
        n /= 1000;
    }
    if (grp_cnt == 0) return 0U;     /* n == 0, not used */

    uint64_t len = 0U;
    bool earlier_groups = (grp_cnt > 1);

    for (int i = grp_cnt - 1; i >= 0; --i) {
        uint64_t g = groups[i];
        if (g == 0) continue;

        /* Add letter count for this group (1‑999) */
        len += group_letter_count(g);

        /* Add scale word if this is not the units group */
        if (i == 1) len += len_thousand;
        else if (i == 2) len += len_million;
        else if (i == 3) len += len_billion;
        /* For groups beyond billions, the correct scale names are not
           defined here; the required sequence only uses much smaller
           numbers, so this limitation is acceptable. */

        /* Insert 'AND' before the units group if necessary */
        if (i == 0 && earlier_groups && g < 100) {
            len += len_and;
        }
    }

    return len;
}

/* Return the nth term of OEIS A072686 (0‑based). */
seq_t a(idx_t n)
{
    static seq_t *good = NULL;
    static size_t good_cap = 0;
    static idx_t good_cnt = 0;
    static uint64_t nextPrime = 2U;

    if (n < good_cnt) return good[n];

    while (good_cnt <= n) {
        /* Find next prime number */
        while (!is_prime((idx_t)nextPrime))
            ++nextPrime;

        /* Count letters in its British English name */
        uint64_t len = number_letter_count(nextPrime);

        /* Keep the prime if the letter count is itself prime */
        if (is_prime_small((seq_t)len)) {
            ensure_capacity(good_cnt + 1, &good_cap, (void **)&good, sizeof(seq_t));
            good[good_cnt++] = (seq_t)nextPrime;
        }

        ++nextPrime;
    }
    return good[n];
}
