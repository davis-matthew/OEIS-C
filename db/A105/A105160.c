/*
 * A105160  Triangle T(n,k) = prime(k) times the n-th term of (3, 2, 7, 5, 13,
 *          11,...) of a permutation of the primes.
 * https://oeis.org/A105160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, tabl
 * Formula:   a(n) = prime(k) * b(r) where n is in row r, column k of the
 *            triangle T(r,k)=prime(k)*b(r). b(r)=prime(r-1) if r is even,
 *            otherwise b(r)=prime(r+1)
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation utilities ---------- */
static size_t prime_count = 0;      /* number of primes stored   */
static seq_t *primes = NULL;       /* primes[0] = 2, primes[i] = (i+1)-th prime */
static uint64_t next_candidate = 3; /* next odd number to test for primality */

/* Ensure that the first 'idx' primes have been generated.
   idx is 1‑based (prime(1)=2). */
static void ensure_prime_index(idx_t idx)
{
    if (!primes) {
        /* initial allocation: store first prime 2 */
        primes = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!primes) return;                     /* allocation failure ignored */
        primes[0] = (seq_t)2;
        prime_count = 1;
        next_candidate = 3;
    }
    while (prime_count < idx) {
        uint64_t cand = next_candidate;
        bool is_prime = true;
        /* trial division by known primes up to sqrt(cand) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = (uint64_t)primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* grow array if necessary */
            if (prime_count + 1 >= prime_count) {
                size_t new_cap = prime_count ? prime_count * 2 : 16;
                seq_t *newpr = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
                if (!newpr) return;        /* allocation failure ignored */
                primes = newpr;
            }
            primes[prime_count] = (seq_t)cand;
            ++prime_count;
        }
        next_candidate += 2;  /* skip even numbers */
    }
}

/* Return the k-th prime (1‑based). */
static seq_t kth_prime(idx_t k)
{
    ensure_prime_index(k);
    return primes[k - 1];
}

/* ---------- find the row containing the n‑th flattened entry ---------- */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n; /* upper bound is n for safety */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit to avoid overflow for large n */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---------- main function implementing the sequence ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset 1: behavior for n < 1 irrelevant */

    /* determine row and column within that row */
    idx_t row = find_row(n);
    /* cumulative count up to previous row: (row-1)*row/2 */
    unsigned __int128 prev128 = (unsigned __int128)(row - 1) * row / 2;
    idx_t prev = (idx_t)prev128;
    idx_t col = n - prev;                    /* 1‑based */

    /* retrieve the required primes */
    seq_t prime_col = kth_prime(col);

    /* compute b(row) according to parity of row */
    seq_t b_row;
    if ((row & 1) == 0) {                    /* even row: prime(row-1) */
        b_row = kth_prime(row - 1);
    } else {                                 /* odd row: prime(row+1) */
        b_row = kth_prime(row + 1);
    }

    return prime_col * b_row;
}
