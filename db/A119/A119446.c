/*
 * A119446  Triangle as described in A100461, except with t(1,n) = prime(n).
 * https://oeis.org/A119446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is produced by the triangular array t(1,n)=prime(n),
 *            t(m+1,n)=(n-m)*floor((t(m,n)-1)/(n-m)). The sequence a is
 *            obtained by reading each row r from left to right (i.e. t(r,r),
 *            t(r-1,r), …, t(1,r)). This function computes a(n) for any n ≥ 1.
 *            It lazily generates the prime numbers required
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Primitive prime list, grown as needed.  Only 64‑bit primes are
   generated – sufficient for n ≤ 10000 (prime(10000) ≈ 104 729).  */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static uint64_t next_prime_candidate = 2;

static uint64_t prime_u64(idx_t n)
{
    if (!prime_list) {
        prime_cap = 32;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }
    while (prime_count < n) {
        uint64_t cand = next_prime_candidate++;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t*)realloc(prime_list,
                                               prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = cand;
        }
    }
    return prime_list[n-1];
}

/* Compute t(m,n) for 1 ≤ m ≤ n.  All arithmetic uses the seq_t type. */
static inline seq_t compute_t(idx_t m, idx_t n)
{
    seq_t cur = (seq_t)prime_u64(n);
    for (idx_t i = 1; i < m; ++i) {
        idx_t div_int = n - i;             /* n-i */
        seq_t div = (seq_t)div_int;
        seq_t tmp = (cur - 1) / div;      /* floor((t(m,n)-1)/(n-m)) */
        cur = div * tmp;
    }
    return cur;
}

/* External linkage required by the kernel. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n.  Iterate until the
       triangular bound is reached; r will never be large in the
       intended usage (≤ 10 000). */
    idx_t r = 1;
    idx_t tri = 1;
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Compute column position within the row. */
    idx_t prev_tri = tri - r;      /* sum of lengths of previous rows */
    idx_t j = n - prev_tri;        /* 1‑based index inside row r */
    idx_t m = r - j + 1;           /* column index used in compute_t */

    return compute_t(m, r);
}
