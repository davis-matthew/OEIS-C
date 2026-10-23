/*
 * A136479  Odd primitive abundant numbers n such that n = x^2 + x + y^2 with
 *          y^2 < 2*x and x and y primes, subsequence of A136476.
 * https://oeis.org/A136479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   _floor_isqrt_u64, factor_u64, mult_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS sequence A136479: odd primitive abundant numbers
 * n = x^2 + x + y^2 with primes x, y and y^2 < 2*x.
 * Values are generated in increasing order and cached between calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/mult_seq.h"

/* ---------- prime sieve ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (prime_list && prime_count && prime_list[prime_count-1] >= limit)
        return;

    /* simple sieve up to limit */
    uint64_t sz = limit + 1;
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp) abort();

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                is_comp[m] = true;
        }
    }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) cnt++;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_list[idx++] = i;

    free(is_comp);
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
}

/* binary search: first prime index > bound */
static size_t upper_bound_prime_idx(uint64_t bound)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= bound)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- heap structure for candidate numbers ---------- */
typedef struct {
    seq_t  n;
    uint32_t x_idx;
    uint32_t y_idx;
    uint32_t max_y_idx;
} Node;

static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static void heap_expand(void)
{
    if (heap_cap == 0) {
        heap_cap = 16;
        heap = (Node *)malloc(heap_cap * sizeof(Node));
    } else {
        heap_cap <<= 1;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    if (!heap) abort();
}

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(Node node)
{
    if (heap_size == heap_cap) heap_expand();
    size_t i = heap_size++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].n <= heap[i].n) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

static void heap_pop(Node *out)
{
    if (heap_size == 0) abort();
    *out = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t best = i;
        if (l < heap_size && heap[l].n < heap[best].n) best = l;
        if (r < heap_size && heap[r].n < heap[best].n) best = r;
        if (best == i) break;
        heap_swap(&heap[best], &heap[i]);
        i = best;
    }
}

static void heap_add_new_x(uint32_t x_idx)
{
    uint64_t x = prime_list[x_idx];
    uint64_t bound = _floor_isqrt_u64(2 * x - 1);
    if (bound < 3) return;            /* no odd y possible */
    size_t max_y_idx = upper_bound_prime_idx(bound);
    if (max_y_idx <= 1) return;       /* no y >=3 */
    uint32_t y_idx = 1;                /* prime 3 */
    uint64_t y = prime_list[y_idx];
    seq_t n_val = (seq_t)((__int128)x * x + x + (__int128)y * y);
    Node node = { n_val, x_idx, y_idx, (uint32_t)max_y_idx };
    heap_push(node);
}

/* ---------- result cache ---------- */
static seq_t *res = NULL;
static size_t res_len = 0;
static size_t res_cap = 0;

static void res_space(size_t need)
{
    if (res_len + need <= res_cap) return;
    size_t new_cap = res_cap ? res_cap * 2 : 16;
    while (res_len + need > new_cap) new_cap <<= 1;
    res = (seq_t *)realloc(res, new_cap * sizeof(seq_t));
    if (!res) abort();
    res_cap = new_cap;
}

static void res_append(seq_t v)
{
    if (res_len > 0 && res[res_len - 1] == v) return; /* avoid duplicate */
    res_space(1);
    res[res_len++] = v;
}

static bool has_abundant_divisor(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                                 uint64_t n, uint64_t d, seq_t sigma_sofar)
{
    if (cnt == 0) { /* leaf */
        if (d <= 1 || d == n) return false;
        return sigma_sofar > (seq_t)(d) * 2;
    }
    uint64_t p = pr[0];
    uint64_t e = ex[0];

    uint64_t p_pow = 1;
    seq_t sigma_p = 1;          /* for exponent 0 */

    for (uint64_t exp = 0; exp <= e; ++exp) {
        uint64_t new_d = d * p_pow;
        seq_t new_sigma = mult_seq(sigma_sofar, sigma_p);
        if (has_abundant_divisor(pr + 1, ex + 1, cnt - 1, n, new_d, new_sigma))
            return true;
        if (exp < e) {
            p_pow *= p;
            sigma_p += p_pow;
        }
    }
    return false;
}

static bool is_primitive_abundant(uint64_t n)
{
    seq_t sigma_n = sigma_of((idx_t)n);
    if (sigma_n <= 2 * (seq_t)n) return false; /* not abundant */

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    return !has_abundant_divisor(pr, ex, cnt, n, 1, 1);
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    /* initialise primes if needed */
    if (!prime_list) {
        uint64_t init_limit = 200000;  /* start with moderate limit */
        ensure_primes_upto(init_limit);
    }

    static uint32_t next_x_idx = 0;

    while (res_len < (size_t)n) {
        /* ensure heap not empty */
        while (heap_size == 0) {
            if (next_x_idx >= prime_count) {
                /* extend prime list */
                uint64_t cur_limit = prime_count ? prime_list[prime_count - 1] * 2 : 100;
                ensure_primes_upto(cur_limit);
            }
            heap_add_new_x(next_x_idx);
            next_x_idx++;
        }

        /* add new x's whose minimal candidate <= current heap minimum */
        while (1) {
            Node *peek = &heap[0];
            while (next_x_idx < prime_count) {
                uint64_t x = prime_list[next_x_idx];
                uint64_t bound = _floor_isqrt_u64(2 * x - 1);
                if (bound < 3) { next_x_idx++; continue; }

                seq_t min_n = (seq_t)((__int128)x * x + x + (__int128)9);
                if (min_n > peek->n) break;
                heap_add_new_x(next_x_idx);
                next_x_idx++;
            }
            break;
        }

        /* pop smallest candidate */
        Node cur;
        heap_pop(&cur);

        uint64_t val64 = (uint64_t)cur.n;    /* assume fits in 64-bit */
        if (is_primitive_abundant(val64))
            res_append(cur.n);

        /* push next y for this x if possible */
        if (cur.y_idx + 1 < cur.max_y_idx) {
            uint32_t ny = cur.y_idx + 1;
            uint64_t y = prime_list[ny];
            uint64_t x = prime_list[cur.x_idx];
            seq_t n_new = (seq_t)((__int128)x * x + x + (__int128)y * y);
            Node new_node = { n_new, cur.x_idx, ny, cur.max_y_idx };
            heap_push(new_node);
        }
    }

    return res[n - 1];
}
