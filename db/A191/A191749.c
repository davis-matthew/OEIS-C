/*
 * A191749  Numbers not the sum of a smaller number and its prime factors
 *          (with multiplicity).
 * https://oeis.org/A191749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer that cannot be written as k + sopfr(k)
   for any k < a(n), where sopfr(k) is the sum of the prime factors of k
   counted with multiplicity (sopfr(1)=0).  The sequence begins
   1,2,3,5,7,9,12,13,16,18,20,21,25,27,28,30,32,37,43,44,45,48,49,50,... */

/* Note: The code uses only integer arithmetic, dynamic allocation,
   and a simple sieve for smallest prime factors.  It memoises the
   sequence in static arrays so that calls may occur in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static storage for the sequence, marked sums, and SPF array */
static seq_t *seq = NULL;          /* sequence values */
static size_t seq_len = 0;         /* number of values stored */
static size_t seq_cap = 0;         /* allocated capacity */

static bool *marked = NULL;        /* positions which are sums k+sopfr(k) */
static size_t marked_cap = 0;      /* capacity of marked array */

static uint64_t *spf = NULL;       /* smallest prime factor array */
static idx_t spf_cap = 0;          /* current maximum index in spf */

/* Next indices to process */
static idx_t next_k = 1;   /* next k for which we need to compute k+sopfr(k) */
static idx_t next_m = 1;   /* next integer to test for membership in the sequence */

/* Ensure that marked[0..idx] exists */
static inline void grow_marked(size_t idx)
{
    if (idx < marked_cap) return;
    size_t new_cap = marked_cap ? marked_cap << 1 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    marked = (bool *)realloc(marked, new_cap * sizeof(bool));
    if (!marked) abort();
    memset(marked + marked_cap, 0, (new_cap - marked_cap) * sizeof(bool));
    marked_cap = new_cap;
}

/* Ensure that seq[0..need-1] exists */
static inline void grow_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

/* Ensure that spf[0..limit] is filled (lazy sieve) */
static inline void ensure_spf(idx_t limit)
{
    if (limit <= spf_cap) return;
    size_t new_cap = spf_cap ? spf_cap : 64;
    while (new_cap < limit) new_cap <<= 1;
    spf = (uint64_t *)realloc(spf, (new_cap + 1) * sizeof(uint64_t));
    if (!spf) abort();
    for (idx_t i = 0; i <= new_cap; ++i) spf[i] = i;
    for (idx_t i = 2; (idx_t)i * i <= new_cap; ++i) {
        if (spf[i] == i) {
            for (idx_t j = (idx_t)i * i; j <= new_cap; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_cap = new_cap;
}

/* Sum of prime factors with multiplicity; sopfr(1)=0 */
static inline seq_t sopfr(idx_t n)
{
    if (n == 1) return (seq_t)0;
    seq_t sum = 0;
    while (n > 1) {
        uint64_t p = spf[n];
        sum += (seq_t)p;
        n /= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset uses a(1) */
    while (seq_len < (size_t)n) {
        /* Mark all sums k + sopfr(k) for k < next_m */
        while (next_k < next_m) {
            ensure_spf(next_k);
            seq_t term = sopfr(next_k);
            idx_t f = next_k + (idx_t)term;
            if (f > next_k) {      /* only mark sums strictly larger than k */
                grow_marked((size_t)f + 1);
                marked[f] = true;
            }
            ++next_k;
        }

        /* Find the next unmarked integer */
        grow_marked((size_t)next_m + 1);
        while (marked[next_m]) {
            ++next_m;
            grow_marked((size_t)next_m + 1);
        }

        /* Record the term */
        grow_seq(seq_len + 1);
        seq[seq_len] = (seq_t)next_m;
        ++seq_len;
        ++next_m;
    }
    return seq[n - 1];
}
