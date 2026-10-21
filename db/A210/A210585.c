/*
 * A210585  List of Lyndon words on {1,...,8} sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A210585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 7716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates Lyndon words over the alphabet {1,…,8} in
   lexicographic order (first by length, then within each length)
   and returns the decimal representation of the n‑th word.
   It uses a straightforward search and a naïve Lyndon check, which
   is fast enough for n up to about 10 000. */

#include <stdint.h>
#include <stdbool.h>

static int mobius(int n)
{
    if (n == 1) return 1;
    int mu = 1;
    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            if (cnt > 1) return 0;
            mu = -mu;
        }
    }
    if (n > 1) mu = -mu;
    return mu;
}

/* count the number of Lyndon words of length len over alphabet {1..8} */
static idx_t count_lyndon_word(idx_t len)
{
    idx_t sum = 0;
    for (int d = 1; d <= (int)len; ++d) {
        if (len % d != 0) continue;
        int mu = mobius(d);
        if (mu == 0) continue;
        idx_t pow = 1;
        idx_t exp = len / d;
        for (idx_t i = 0; i < exp; ++i) pow *= (idx_t)8;
        sum += (idx_t)mu * pow;
    }
    return sum / len;
}

/* test whether w[0..len-1] is a Lyndon word (strictly smaller than all rotations) */
static bool is_lyndon(const unsigned char *w, int len)
{
    for (int shift = 1; shift < len; ++shift) {
        int i;
        for (i = 0; i < len; ++i) {
            unsigned char a = w[i];
            unsigned char b = w[(i + shift) % len];
            if (a < b) {            /* this rotation is larger */
                break;
            } else if (a > b) {      /* this rotation is smaller */
                return false;
            }
        }
        if (i == len) return false; /* rotation identical -> not Lyndon */
    }
    return true;
}

/* depth‑first search over words of a fixed length, lexicographically,
   counting Lyndon words until the target is reached */
static void dfs_find(idx_t pos, int len, unsigned char *w,
                     idx_t target, idx_t *seen, seq_t *result,
                     bool *found)
{
    if (*found) return;
    if (pos == len) {
        if (is_lyndon(w, len)) {
            ++(*seen);
            if (*seen == target) {
                seq_t val = 0;
                for (int i = 0; i < len; ++i) {
                    val = val * 10 + (seq_t)w[i];
                }
                *result = val;
                *found = true;
            }
        }
        return;
    }
    for (int d = 1; d <= 8; ++d) {
        w[pos] = (unsigned char)d;
        dfs_find(pos + 1, len, w, target, seen, result, found);
        if (*found) return;
    }
}

seq_t a(idx_t n)
{
    const int alphabet_size = 8;
    idx_t rem = n;
    int len = 1;

    /* find the word length that contains the n-th word */
    while (true) {
        idx_t cnt = count_lyndon_word((idx_t)len);
        if (rem <= cnt) break;
        rem -= cnt;
        ++len;
    }

    unsigned char word[32];        /* 32 > max length we need */
    idx_t seen = 0;
    seq_t result = 0;
    bool found = false;
    dfs_find(0, len, word, rem, &seen, &result, &found);

    return result;
}
