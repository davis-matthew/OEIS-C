/*
 * A123364  Primes of the form a^2 + b^3 (with repetition).
 * https://oeis.org/A123364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form a² + b³ (with repetition),
 *            sorted ascending
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 69 terms through n = 195 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

#define BMAX 500

typedef struct {
    seq_t val;
    uint32_t b;
    uint32_t a;
} Node;

static Node heap[BMAX];
static size_t heap_cnt = 0;
static bool heap_inited = false;

static seq_t *primes = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;

static void heap_sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static void heap_sift_down(size_t i) {
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_cnt) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_cnt && heap[right].val < heap[left].val) smallest = right;
        if (heap[i].val <= heap[smallest].val) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(uint32_t b, uint32_t a, seq_t val) {
    heap[heap_cnt].val = val;
    heap[heap_cnt].b   = b;
    heap[heap_cnt].a   = a;
    ++heap_cnt;
    heap_sift_up(heap_cnt - 1);
}

static Node heap_pop(void) {
    Node res = heap[0];
    heap[0] = heap[heap_cnt - 1];
    --heap_cnt;
    heap_sift_down(0);
    return res;
}

static void init(void) {
    if (heap_inited) return;
    heap_inited = true;
    for (uint32_t b = 1; b <= BMAX; ++b) {
        seq_t val = (seq_t)b * (seq_t)b * (seq_t)b + (seq_t)1; // a = 1
        heap_push(b, 1, val);
    }
}

static void ensure_primes_up_to(idx_t n) {
    init();
    while (prime_cnt < (size_t)n) {
        Node cur = heap_pop();
        if (is_prime_big(cur.val)) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 8;
                seq_t *new_arr = seq_realloc(primes, prime_cap, new_cap);
                if (!new_arr) return; /* Allocation failed – should not happen */
                primes = new_arr;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = cur.val;
        }
        uint32_t a_next = cur.a + 1;
        seq_t a_sq = (seq_t)a_next * (seq_t)a_next;
        seq_t b3   = (seq_t)cur.b * (seq_t)cur.b * (seq_t)cur.b;
        seq_t next_val = a_sq + b3;
        heap_push(cur.b, a_next, next_val);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_primes_up_to(n);
    return primes[n - 1];
}
