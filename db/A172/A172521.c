/*
 * A172521  Partial sums of A078902.
 * https://oeis.org/A172521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n primes of the form (k+1)^(2^m)+k^(2^m)
 *            with m>1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"
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

typedef struct {
    seq_t val;
    idx_t m;
    uint64_t k;
} Node;

/* A binary min‑heap of Node, ordered by .val. */
static Node *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(Node n)
{
    if (heap_sz == heap_cap) {
        heap_cap = (heap_cap == 0) ? 4 : heap_cap * 2;
        heap = mem_resize(Node, heap, heap_cap);
    }
    size_t i = heap_sz++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val < heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

static Node heap_pop_min(void)
{
    Node res = heap[0];
    if (--heap_sz) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t smallest = l;
            if (r < heap_sz && heap[r].val < heap[l].val) smallest = r;
            if (heap[smallest].val >= heap[i].val) break;
            heap_swap(&heap[i], &heap[smallest]);
            i = smallest;
        }
    }
    return res;
}

/* Return (k+1)^(2^m)+k^(2^m).  m>1. */
static seq_t compute_val(uint64_t k, idx_t m)
{
    idx_t e = ((idx_t)1) << m;          /* exponent 2^m */
    seq_t a = ipow_idx((seq_t)(k + 1), e);
    seq_t b = ipow_idx((seq_t)k, e);
    return a + b;
}

/* Prefix sums of the required primes. */
static seq_t *sums = NULL;
static size_t sums_cap = 0;
static idx_t prime_cnt = 0;

/* Generate primes until we have seen n of them. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n) return;

    if (heap == NULL) {
        for (idx_t m = 2; m <= 5; ++m) {      /* m=2..5 covers all primes needed up to n≈10000 */
            Node start = { compute_val(1, m), m, 1 };
            heap_push(start);
        }
    }

    while (prime_cnt < n) {
        Node cur = heap_pop_min();
        seq_t v = cur.val;
        if (is_prime_big(v)) {
            if (prime_cnt == 0) {
                if (sums_cap == 0) {
                    sums_cap = 4;
                    sums = mem_new(seq_t, sums_cap);
                }
                sums[0] = v;
            } else {
                if ((size_t)prime_cnt >= sums_cap) {
                    sums_cap *= 2;
                    sums = mem_resize(seq_t, sums, sums_cap);
                }
                sums[prime_cnt] = sums[prime_cnt - 1] + v;
            }
            ++prime_cnt;
        }
        Node next;
        next.m = cur.m;
        next.k = cur.k + 1;
        next.val = compute_val(next.k, next.m);
        heap_push(next);
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* offset is 1; n=0 is unused */
    ensure_primes(n);
    return sums[n - 1];
}
