/*
 * A086455  Sum of divisors of prime powers: sigma(p^e).
 * https://oeis.org/A086455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(p^e) where p^e is the n‑th smallest prime power
 *            (i.e. A000961). sigma(p^e) = (p^(e+1) – 1)/(p – 1)
 * Range:     u8 n <= 11, u16 n <= 1218, u32 n <= 7693, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

/* ---------- Min‑heap of current prime powers ---------- */
struct prime_heap {
    seq_t  *pwr;    /* current power p^e */
    uint32_t *pr;  /* corresponding prime p */
    size_t sz;
    size_t cap;
} heap = {NULL, NULL, 0, 0};

/* Resize heap storage.  Needed when compiling as C++ (realloc returns void*). */
static void heap_resize(size_t new_cap) {
    heap.pwr = (seq_t*)realloc(heap.pwr, new_cap * sizeof(seq_t));
    heap.pr  = (uint32_t*)realloc(heap.pr,  new_cap * sizeof(uint32_t));
    heap.cap = new_cap;
}

static void heap_push(uint32_t p, seq_t v) {
    if (heap.sz == heap.cap) {
        size_t new_cap = heap.cap ? heap.cap * 2 : 8;
        heap_resize(new_cap);
    }
    size_t i = heap.sz;
    heap.pwr[heap.sz] = v;
    heap.pr[heap.sz]  = p;
    ++heap.sz;
    /* sift‑up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap.pwr[parent] <= heap.pwr[i]) break;
        seq_t tv = heap.pwr[parent]; heap.pwr[parent] = heap.pwr[i]; heap.pwr[i] = tv;
        uint32_t tp = heap.pr[parent]; heap.pr[parent] = heap.pr[i]; heap.pr[i] = tp;
        i = parent;
    }
}

static void heap_pop(uint32_t *p_out, seq_t *v_out) {
    if (heap.sz == 0) return;            /* should never happen */
    *p_out = heap.pr[0];
    *v_out = heap.pwr[0];
    --heap.sz;
    if (heap.sz > 0) {
        heap.pwr[0] = heap.pwr[heap.sz];
        heap.pr[0]  = heap.pr[heap.sz];
        /* sift‑down */
        size_t i = 0;
        while (1) {
            size_t left  = i * 2 + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left < heap.sz  && heap.pwr[left]  < heap.pwr[smallest]) smallest = left;
            if (right < heap.sz && heap.pwr[right] < heap.pwr[smallest]) smallest = right;
            if (smallest == i) break;
            seq_t tv = heap.pwr[i]; heap.pwr[i] = heap.pwr[smallest]; heap.pwr[smallest] = tv;
            uint32_t tp = heap.pr[i]; heap.pr[i] = heap.pr[smallest]; heap.pr[smallest] = tp;
            i = smallest;
        }
    }
}

/* ---------- Prime generation ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t next_candidate = 3;   /* start after 2 */

static void append_prime(uint32_t p) {
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        prime_list = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

static uint32_t next_prime(void) {
    if (prime_cnt == 0) {              /* first prime is 2 */
        append_prime(2);
        return 2;
    }
    for (;;) {
        uint64_t n = next_candidate;
        ++next_candidate;
        bool prime = true;
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
            if (n % prime_list[i] == 0) { prime = false; break; }
        }
        if (prime) {
            append_prime((uint32_t)n);
            return (uint32_t)n;
        }
    }
}

static void ensure_primes_upto(seq_t limit) {
    while ((seq_t)next_candidate <= limit) {
        uint32_t p = next_prime();          /* also appends to prime_list */
        heap_push(p, (seq_t)p);             /* first power is p^1 = p */
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;

    /* store the result for index n (1‑based) */
    if (n == 0) return 0;   /* undefined, but avoid UB */

    /* ensure cache can hold n terms */
    if (cache_sz < (size_t)n)
        ensure_cache((idx_t)n, &cache, &cache_cap);

    /* generate terms up to needed index */
    while (cache_sz < (size_t)n) {
        /* the very first term a(1) = 1 (sigma(1)) */
        if (cache_sz == 0) {
            cache[cache_sz++] = 1;
            continue;
        }

        /* make sure at least one prime is in the heap */
        if (heap.sz == 0) {
            uint32_t p = next_prime();   /* returns 2 and adds to list */
            heap_push(p, (seq_t)p);
        }

        /* Insert new primes whose first power is <= current minimum in heap */
        seq_t current_min = heap.pwr[0];
        ensure_primes_upto(current_min);

        /* extract the smallest prime power */
        uint32_t p;
        seq_t val;
        heap_pop(&p, &val);

        /* compute sigma(p^e) = (p^(e+1) – 1)/(p – 1) */
        seq_t next_val = val * (seq_t)p;          /* p^(e+1) */
        seq_t sigma = (next_val - 1) / ((seq_t)p - 1);
        cache[cache_sz++] = sigma;

        /* push the next power of this prime back into the heap */
        heap_push(p, next_val);
    }

    return cache[n - 1];
}
