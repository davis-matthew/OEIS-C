/*
 * A023096  Least k such that k and 4k are anagrams in base n (written in base
 *          10).
 * https://oeis.org/A023096
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS Formula: a(n) = least k such that the base‑n digit multiset of k
 * equals that of 4·k.
 *
 * Implementation detail:  k must satisfy k ≡ 0 (mod (n‑1)/gcd(n‑1,3))
 * because the digit sum of a number in base n is congruent to the
 * number modulo (n‑1).  This reduces the search space from all k to
 * multiples of step = (n‑1)/gcd(n‑1,3).  For every such k the
 * multisets are compared by sorting the digit arrays of k and 4·k.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* In‑place insertion sort of up to 128 unsigned shorts. */
static void sort_digits(unsigned short *a, unsigned len)
{
    for (unsigned i = 1; i < len; ++i) {
        unsigned short key = a[i];
        unsigned j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

/* Extract base‐n digits of value into buf (least significant first).
   *len is set to the number of digits.  For zero the digit 0 is returned. */
static void get_digits(seq_t value, idx_t base,
                       unsigned short *buf, unsigned *len)
{
    unsigned pos = 0;
    if (value == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (value != 0) {
        seq_t rem = value % base;
        buf[pos++] = (unsigned short)rem;
        value /= base;
    }
    *len = pos;
}

/* Compare the digit multisets of a and b in base n. */
static bool digits_equal(seq_t a, seq_t b, idx_t base)
{
    unsigned short da[128], db[128];
    unsigned la, lb;

    get_digits(a, base, da, &la);
    get_digits(b, base, db, &lb);

    if (la != lb) return false;

    sort_digits(da, la);
    sort_digits(db, lb);

    for (unsigned i = 0; i < la; ++i)
        if (da[i] != db[i]) return false;

    return true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;  // behaviour for n<5 is undefined
    /* Step ensuring k ≡ 0 (mod (n‑1)/gcd(n‑1,3)) */
    idx_t step = (n - 1) / gcd_idx(n - 1, (idx_t)3);

    for (idx_t k = step; ; k += step) {
        seq_t kv = (seq_t)k;
        seq_t fk = kv * 4;
        if (digits_equal(kv, fk, n))
            return kv;
    }
}
