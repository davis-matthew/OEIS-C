/*
 * A106727  Triangle T(n,k) = (f(n+1)*f(k+1) mod 10), where f(j) = 10 -
 *          (prime(j+3) mod 10), read by rows.
 * https://oeis.org/A106727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, less, tabl
 * Formula:   a(n) = T(r, c) = f(r+1) * f(c+1) mod 10, where f(j) = 10 –
 *            (prime(j+3) mod 10) and (r, c) are the row and column of the
 *            n‑th element of the triangle read row‑by‑row. Offset = 0
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Local notes for the implementation
    ------------------------------------
    •  The algorithm builds a small table of primes on demand.
    •  For each n it finds the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2,
        then the column c = n – r(r+1)/2.
    •  f(r+1) and f(c+1) are computed from the primes table.
    •  The product modulo 10 is returned as the sequence value.
    •  All arithmetic involving sequence values is done in seq_t,
        indices use idx_t.  No floating‑point or large intermediate
        values escape because 10 is tiny.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime table – allocate a modest amount of primes for the typical   */
/* range of n (up to at least 10 000).                                 */
/* ------------------------------------------------------------------ */
#define PRIME_TABLE_SIZE 20000          /* plenty for n ≤ 10 000 */
static uint64_t prime_tbl[PRIME_TABLE_SIZE];
static size_t   prime_cnt = 0;         /* number of primes stored */

/* Ensure that prime_tbl has at least 'need' primes (1‑indexed). */
static void
ensure_primes (size_t need)
{
    if (prime_cnt >= need)
        return;

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_tbl[prime_cnt - 1] + 1;

    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= PRIME_TABLE_SIZE) {
                /* Should never happen for the problem limits. */
                return;
            }
            prime_tbl[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Find the row number r for the n‑th element of the triangle.   */
/*  r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2.                          */
/* ------------------------------------------------------------------ */
static idx_t
row_of_index (idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((hi * (hi + 1) / 2) <= n)
        hi <<= 1;                     /* exponential growth */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((mid * (mid + 1) / 2) <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------------------------------------------------ */
/*  f(j) = 10 – (prime(j+3) mod 10), for j ≥ 1.   */
/* ------------------------------------------------------------------ */
static seq_t
f_val (idx_t j)
{
    /*  j is at least 1.  Need the prime at position j+3.  */
    size_t need = (size_t)(j + 3);
    ensure_primes(need);
    uint64_t p = prime_tbl[need - 1];          /* prime(j+3) */
    return (seq_t)(10 - (p % 10));
}

/* ------------------------------------------------------------------ */
/*  Public sequence entry.  */
seq_t a (idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t c = n - r * (r + 1) / 2;          /* column in the row */

    seq_t fr = f_val(r + 1);                /* f(r+1) */
    seq_t fc = f_val(c + 1);                /* f(c+1) */

    return (fr * fc) % 10;
}
