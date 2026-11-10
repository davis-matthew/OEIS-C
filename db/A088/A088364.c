/*
 * A088364  a(n) = index of the n-th local minimum of A053707.
 * https://oeis.org/A088364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th local minimum of A053707, found by a *
 *            search: i is counted when A053707(i) < A053707(i+1) and (i = 1
 *            or * A053707(i-1) > A053707(i)), where A053707(i) = A025475(i+1)
 *            - A025475(i). * A025475 (prime powers p^k, k >= 2, with 1) comes
 *            from a min-heap into * which each prime'...
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 5635, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9628 (2026-11-07)
 * Helpers:   build_primes, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/mem.h"

typedef struct {
    seq_t val;
    idx_t prime;
} P_Node;

static struct {
    P_Node *data;
    size_t size;
    size_t cap;
} P_heap = { NULL, 0, 0 };

static void P_heap_swap(size_t i, size_t j)
{
    P_Node tmp = P_heap.data[i];
    P_heap.data[i] = P_heap.data[j];
    P_heap.data[j] = tmp;
}

static void P_heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (!(P_heap.data[i].val < P_heap.data[parent].val))
            break;
        P_heap_swap(i, parent);
        i = parent;
    }
}

static void P_heap_sift_down(size_t i)
{
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < P_heap.size && P_heap.data[left].val < P_heap.data[smallest].val)
            smallest = left;
        if (right < P_heap.size && P_heap.data[right].val < P_heap.data[smallest].val)
            smallest = right;
        if (smallest == i)
            break;
        P_heap_swap(i, smallest);
        i = smallest;
    }
}

static void P_heap_push(P_Node node)
{
    if (P_heap.size == P_heap.cap) {
        size_t new_cap = P_heap.cap ? (P_heap.cap << 1) : 64;
        P_Node *tmp = mem_resize(P_Node, P_heap.data, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        P_heap.data = tmp;
        P_heap.cap = new_cap;
    }
    P_heap.data[P_heap.size++] = node;
    P_heap_sift_up(P_heap.size - 1);
}

static P_Node P_heap_pop_min(void)
{
    P_Node min = P_heap.data[0];
    P_heap.data[0] = P_heap.data[--P_heap.size];
    P_heap_sift_down(0);
    return min;
}

static struct {
    seq_t *data;
    size_t len;
    size_t cap;
} P_powers = { NULL, 0, 0 };

static idx_t *P_primes = NULL;
static size_t P_nprimes = 0;
static idx_t P_plimit = 0;
static size_t P_next_prime = 0;   /* index of next prime whose square is not yet in heap */

static idx_t P_get_prime(size_t k)
{
    while (k >= P_nprimes) {
        idx_t newlim = P_plimit ? P_plimit * 2 : 65536;
        idx_t *np = NULL;
        size_t cnt = 0;
        build_primes(newlim, &np, &cnt);
        if (P_primes) free(P_primes);
        P_primes = np;
        P_nprimes = cnt;
        P_plimit = newlim;
    }
    return P_primes[k];
}

static seq_t A025475_at(size_t k)   /* 0-based: k=0 -> 1 */
{
    if (P_powers.data == NULL) {
        P_powers.cap = 256;
        P_powers.data = mem_new(seq_t, P_powers.cap);
        if (!P_powers.data) exit(EXIT_FAILURE);
        P_powers.data[0] = (seq_t)1;
        P_powers.len = 1;
    }
    while (P_powers.len <= k) {
        /* make sure every prime square that could be the next minimum is in the heap */
        while (1) {
            idx_t p = P_get_prime(P_next_prime);
            seq_t sq = (seq_t)p * (seq_t)p;
            if (P_heap.size == 0 || sq <= P_heap.data[0].val) {
                P_Node nd;
                nd.val = sq;
                nd.prime = p;
                P_heap_push(nd);
                P_next_prime++;
            } else {
                break;
            }
        }
        P_Node cur = P_heap_pop_min();
        if (P_powers.len == P_powers.cap) {
            size_t nc = P_powers.cap << 1;
            seq_t *tmp = mem_resize(seq_t, P_powers.data, nc);
            if (!tmp) exit(EXIT_FAILURE);
            P_powers.data = tmp;
            P_powers.cap = nc;
        }
        P_powers.data[P_powers.len++] = cur.val;

        seq_t pp = (seq_t)cur.prime;
        seq_t nxt = cur.val * pp;
        if (nxt / pp == cur.val) {   /* skip on overflow */
            P_Node nd;
            nd.val = nxt;
            nd.prime = cur.prime;
            P_heap_push(nd);
        }
    }
    return P_powers.data[k];
}

static seq_t A053707_a(idx_t n)
{
    return A025475_at((size_t)n) - A025475_at((size_t)n - 1);
}

static struct {
    idx_t *data;
    size_t len;
    size_t cap;
    idx_t next;
} M = { NULL, 0, 0, 1 };

seq_t a(idx_t n)
{
    while (M.len < (size_t)n) {
        idx_t i = M.next++;
        seq_t cur = A053707_a(i);
        seq_t nxt = A053707_a(i + 1);
        bool ok = (cur < nxt);
        if (ok && i > 1) {
            seq_t prv = A053707_a(i - 1);
            ok = (prv > cur);
        }
        if (ok) {
            if (M.len == M.cap) {
                size_t nc = M.cap ? M.cap << 1 : 64;
                idx_t *tmp = (idx_t *)realloc(M.data, nc * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                M.data = tmp;
                M.cap = nc;
            }
            M.data[M.len++] = i;
        }
    }
    return (seq_t)M.data[n - 1];
}
