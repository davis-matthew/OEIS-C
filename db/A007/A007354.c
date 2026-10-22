/*
 * A007354  Where the prime race among 7k+1, ..., 7k+6 changes leader.
 * https://oeis.org/A007354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sieve primes in segments and keep a running count of primes in each class 7k+r, r = 1..6.
   The leader is the class with the most primes, and it changes only when another class gets strictly
   more primes than it. A tie leaves the leader as it is. a(n) is the prime at which the n-th change
   happens; the first is p = 2, which makes class 2 the first leader. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A7354_BASE_LIM (1u << 20)
#define A7354_SEG (1u << 20)

static uint32_t *a7354_bp = NULL;
static size_t a7354_nbp = 0;
static uint64_t *a7354_terms = NULL;
static size_t a7354_cnt = 0, a7354_cap = 0;
static uint64_t a7354_counts[7];
static int a7354_leader = 0;
static uint64_t a7354_next = 2;

static void a7354_init(void) {
    unsigned char *s = (unsigned char *)calloc(A7354_BASE_LIM + 1, 1);
    size_t i, j, c = 0;
    for (i = 2; i * i <= A7354_BASE_LIM; i++)
        if (!s[i])
            for (j = i * i; j <= A7354_BASE_LIM; j += i) s[j] = 1;
    for (i = 2; i <= A7354_BASE_LIM; i++) if (!s[i]) c++;
    a7354_bp = (uint32_t *)malloc(c * sizeof(uint32_t));
    a7354_nbp = 0;
    for (i = 2; i <= A7354_BASE_LIM; i++) if (!s[i]) a7354_bp[a7354_nbp++] = (uint32_t)i;
    free(s);
    memset(a7354_counts, 0, sizeof(a7354_counts));
}

static void a7354_push(uint64_t p) {
    if (a7354_cnt == a7354_cap) {
        a7354_cap = a7354_cap ? a7354_cap * 2 : 1024;
        a7354_terms = (uint64_t *)realloc(a7354_terms, a7354_cap * sizeof(uint64_t));
    }
    a7354_terms[a7354_cnt++] = p;
}

static void a7354_extend(void) {
    static unsigned char mark[A7354_SEG];
    uint64_t lo = a7354_next, hi = lo + A7354_SEG;
    size_t k;
    uint64_t x;
    memset(mark, 0, sizeof(mark));
    for (k = 0; k < a7354_nbp; k++) {
        uint64_t p = a7354_bp[k];
        uint64_t st;
        if (p * p >= hi) break;
        st = ((lo + p - 1) / p) * p;
        if (st < p * p) st = p * p;
        for (x = st; x < hi; x += p) mark[x - lo] = 1;
    }
    for (x = lo; x < hi; x++) {
        int r;
        if (mark[x - lo] || x < 2) continue;
        r = (int)(x % 7);
        if (r == 0) continue;
        a7354_counts[r]++;
        if (a7354_leader == 0) {
            a7354_leader = r;
            a7354_push(x);
        } else if (r != a7354_leader && a7354_counts[r] > a7354_counts[a7354_leader]) {
            a7354_leader = r;
            a7354_push(x);
        }
    }
    a7354_next = hi;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!a7354_bp) a7354_init();
    while ((idx_t)a7354_cnt < n) a7354_extend();
    return (seq_t)a7354_terms[n - 1];
}
