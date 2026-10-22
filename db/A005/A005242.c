/*
 * A005242  Lexicographically earliest sequence of distinct positive integers
 *          such that sums of two or three consecutive earlier terms are
 *          excluded.
 * https://oeis.org/A005242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a greedy search: a(n) is the smallest positive
 *            integer that is not an earlier term and not a sum of two or
 *            three consecutive earlier terms
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long *a005242_terms = 0;
static idx_t a005242_count = 0;
static idx_t a005242_cap = 0;
static unsigned char *a005242_blocked = 0;
static unsigned long long a005242_bsize = 0;
static unsigned long long a005242_cur = 1;

static void a005242_ensure_blocked(unsigned long long need) {
    if (need < a005242_bsize) return;
    unsigned long long ns = a005242_bsize ? a005242_bsize : 1024;
    while (ns <= need) ns *= 2;
    a005242_blocked = (unsigned char *)realloc(a005242_blocked, (size_t)ns);
    memset(a005242_blocked + a005242_bsize, 0, (size_t)(ns - a005242_bsize));
    a005242_bsize = ns;
}

static void a005242_mark(unsigned long long v) {
    a005242_ensure_blocked(v);
    a005242_blocked[v] = 1;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a005242_count) {
        if (n + 1 > a005242_cap) {
            idx_t nc = a005242_cap ? a005242_cap : 64;
            while (nc < n + 1) nc *= 2;
            a005242_terms = (unsigned long long *)realloc(a005242_terms,
                                (size_t)nc * sizeof(unsigned long long));
            a005242_cap = nc;
        }
        while (a005242_count < n) {
            a005242_ensure_blocked(a005242_cur);
            while (a005242_blocked[a005242_cur]) {
                a005242_cur++;
                a005242_ensure_blocked(a005242_cur);
            }
            unsigned long long v = a005242_cur;
            a005242_count++;
            a005242_terms[a005242_count] = v;
            a005242_mark(v);
            if (a005242_count >= 2) {
                unsigned long long s2 = v + a005242_terms[a005242_count - 1];
                a005242_mark(s2);
                if (a005242_count >= 3)
                    a005242_mark(s2 + a005242_terms[a005242_count - 2]);
            }
        }
    }
    return (seq_t)a005242_terms[n];
}
