/*
 * A023099  a(n) is least k such that k and 7k are anagrams in base n (written
 *          in base 10).
 * https://oeis.org/A023099
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): least k such that k and 7·k are anagrams in base n.
   The algorithm tests k in multiples of (n‑1)/gcd(6, n‑1) — a necessary
   condition coming from digit sums modulo (n‑1).  For each candidate
   k it compares the sorted digit lists of k and 7·k in base n.  This
   is O(current minimal k * log(minimal k)/n) and fast enough for
   n up to about 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Small gcd for uint64_t */
static inline uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Comparator for qsort on uint32_t array */
static int cmp_uint32(const void *a, const void *b) {
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return (va > vb) - (va < vb);
}

/* Count digits of x in base 'base' storing in buf[0..len-1] (least significant first) */
static size_t get_digits(seq_t x, idx_t base, uint32_t *buf, size_t maxlen) {
    size_t i = 0;
    seq_t b = (seq_t)base;
    if (x == 0) {
        buf[0] = 0;
        return 1;
    }
    while (x != 0) {
        if (i >= maxlen) break;            /* safety, should not happen */
        buf[i++] = (uint32_t)(x % b);
        x = x / b;
    }
    return i;
}

/* Check whether k and 7*k are anagrams in base n */
static bool anagram_in_base(seq_t k, idx_t n) {
    /* Max digits for n up to at least 10000 and k up to ~1e15 is < 256 */
    uint32_t digits_k[256];
    uint32_t digits_7k[256];
    size_t len_k = get_digits(k, n, digits_k, 256);
    size_t len_7k = get_digits(k * (seq_t)7, n, digits_7k, 256);
    if (len_k != len_7k) return false;
    qsort(digits_k, len_k, sizeof(uint32_t), cmp_uint32);
    qsort(digits_7k, len_7k, sizeof(uint32_t), cmp_uint32);
    return memcmp(digits_k, digits_7k, len_k * sizeof(uint32_t)) == 0;
}

seq_t a(idx_t n) {
    if (n < 8) return 0;            /* undefined below offset */
    /* step = (n-1)/gcd(6, n-1) */
    uint64_t g = gcd64((uint64_t)(n - 1), 6ULL);
    seq_t step = (seq_t)((n - 1) / g);
    if (step == 0) return 0;          /* safety for tiny seq_t */

/*  We'll iterate over k = step, 2*step, ... until we find an anagram.
    To avoid infinite loops if seq_t overflows, stop when the counter
    wraps back to zero. */
    seq_t k = step;
    while (true) {
        if (anagram_in_base(k, n)) return k;
        seq_t next = k + step;
        if (next <= k) return 0;    /* overflow or loop would repeat */
        k = next;
    }
}
