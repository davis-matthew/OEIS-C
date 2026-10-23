/*
 * A020628  Smallest nonempty set S containing prime divisors of 9k+6 for each
 *          k in S.
 * https://oeis.org/A020628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A020628: Smallest nonempty set S containing prime divisors of 9k+6
   for each k in S.   a(n) = the n‑th prime in S, sorted increasingly. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dynamic storage of the discovered primes (sorted) */
static idx_t   *S_vals = NULL;   /* sorted list of primes in S */
static idx_t    S_len  = 0;       /* number of elements currently stored */
static idx_t    S_cap  = 0;       /* allocated capacity of S_vals */

/* Queue of primes to be processed */
static idx_t   *queue  = NULL;    /* queue buffer */
static idx_t    q_head = 0;       /* index of next element to pop */
static idx_t    q_tail = 0;       /* index one past last element */
static idx_t    q_cap  = 0;       /* allocated capacity of queue */

/* Bitmap of primes already present in S */
static uint8_t *present = NULL;   /* 0/1 indexed by prime value */
static idx_t    present_limit = 0;/* size of present array - 1 */

/* Ensure capacity of S_vals */
static void ensure_S_cap(idx_t need)
{
    if (need <= S_cap) return;
    size_t new_cap = S_cap ? S_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    S_vals = (idx_t *)realloc(S_vals, new_cap * sizeof(idx_t));
    S_cap  = new_cap;
}

/* Insert a prime into S_vals while keeping it sorted */
static void insert_sorted(idx_t val)
{
    ensure_S_cap(S_len + 1);
    idx_t lo = 0, hi = S_len;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (S_vals[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    if (lo < S_len)
        memmove(&S_vals[lo + 1], &S_vals[lo], (S_len - lo) * sizeof(idx_t));
    S_vals[lo] = val;
    ++S_len;
}

/* Ensure capacity of the queue */
static void ensure_q_cap(idx_t need)
{
    if (need <= q_cap) return;
    size_t new_cap = q_cap ? q_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    queue = (idx_t *)realloc(queue, new_cap * sizeof(idx_t));
    q_cap  = new_cap;
}

/* Push a prime onto the queue */
static void push_queue(idx_t val)
{
    ensure_q_cap(q_tail + 1);
    queue[q_tail++] = val;
}

/* Ensure present array large enough to index up to 'limit' */
static void ensure_present_limit(idx_t limit)
{
    if (limit <= present_limit) return;
    size_t new_limit = present_limit ? present_limit * 2 : 1;
    while (new_limit < limit) new_limit = new_limit * 2 + 1024;
    uint8_t *new_present = (uint8_t *)calloc(new_limit + 1, 1);
    if (present) memcpy(new_present, present,
                        (size_t)(present_limit + 1));
    free(present);
    present     = new_present;
    present_limit = new_limit;
}

/* Add a prime to the set if not already present */
static void add_prime(idx_t p)
{
    if (p > present_limit) ensure_present_limit(p);
    if (!present[p]) {
        present[p] = 1;
        push_queue(p);
        insert_sorted(p);
    }
}

/* Factorise n = 9*k+6 by trial division and add prime factors */
static void factor_9k_plus_6(idx_t k)
{
    unsigned __int128 tmp = (unsigned __int128)9 * (unsigned __int128)k + 6;
    uint64_t n = (uint64_t)tmp;          /* fits into 64‑bit for the needed range */
    uint64_t temp = n;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            add_prime(d);
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) add_prime(temp);
}

/* Compute the nth prime in S, 1‑based */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Initialise and fully expand the set if not yet done */
    if (S_len == 0) {
        add_prime(3);           /* seed value */
        /* process all currently queued primes until queue empty */
        while (q_head < q_tail) {
            idx_t k = queue[q_head++];
            factor_9k_plus_6(k);
        }
    }

    /* If the sequence seems infinite we keep processing until we have n terms */
    while (S_len < n &&
           q_head < q_tail) {
        idx_t k = queue[q_head++];
        factor_9k_plus_6(k);
    }

    /* If we don't have enough terms but the queue is empty, the set is finite
       and further n are undefined.  We simply return 0 in that case. */
    if (n > S_len) return 0;

    return (seq_t)S_vals[n - 1];
}
