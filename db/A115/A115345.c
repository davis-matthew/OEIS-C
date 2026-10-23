/*
 * A115345  Position of n^4 in the lexicographically arranged permutations of
 *          digits of n^4.
 * https://oeis.org/A115345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rank of the base‑10 representation of n^4 among all
 *            permutations of its digits sorted lexicographically
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Position of n^4 in the lexicographically arranged permutations of digits of n^4 */

#include <stddef.h>
#include <stdbool.h>

/* Auxiliary: factorials up to 20 (enough for n up to about 10000, since n^4 < 10^17). */
static seq_t fact[21] = {0};

static bool fact_initialised = false;

/* Compute factorials once. */
static void init_fact(void) {
    if (fact_initialised) return;
    fact[0] = 1;
    for (int i = 1; i <= 20; ++i) fact[i] = fact[i - 1] * (seq_t)i;
    fact_initialised = true;
}

/* Count permutations of a multiset given counts and total length. */
static seq_t multiset_perm(int *cnt, int total) {
    seq_t res = fact[total];
    for (int d = 0; d <= 9; ++d) {
        int c = cnt[d];
        if (c > 1) res /= fact[c];
    }
    return res;
}

/* Main function. */
seq_t a(idx_t n) {
    init_fact();

    /* m = n^4 */
    seq_t m = (seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n;

    /* Extract digits of m into array, most significant first. */
    uint8_t digitbuf[64];
    int len = 0;
    seq_t tmp = m;
    if (tmp == 0) {
        digitbuf[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digitbuf[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to get most significant first. */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t t = digitbuf[i];
            digitbuf[i] = digitbuf[len - 1 - i];
            digitbuf[len - 1 - i] = t;
        }
    }

    /* Count digit frequencies. */
    int cnt[10] = {0};
    for (int i = 0; i < len; ++i) cnt[digitbuf[i]]++;

    /* Compute lexicographic rank. */
    seq_t rank = 1;  /* ranks start at 1 */
    for (int pos = 0; pos < len; ++pos) {
        int cur = digitbuf[pos];
        /* For each smaller digit still available, add number of permutations after placing it. */
        for (int d = 0; d < cur; ++d) {
            if (cnt[d] == 0) continue;
            cnt[d]--;  /* simulate placing digit d */
            rank += multiset_perm(cnt, len - pos - 1);
            cnt[d]++;  /* restore */
        }
        /* Consume the actual digit. */
        cnt[cur]--;
    }

    return rank;
}
