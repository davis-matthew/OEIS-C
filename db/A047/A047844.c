/*
 * A047844  Patrick De Geest's "Generations" array read by antidiagonals:
 *          a(n,1) = n-th prime, a(1,k+1) = a(2,k), a(n,k+1) = a(n-1,k) +
 *          a(n+1,k).
 * https://oeis.org/A047844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 134, u32 n <= 526, u64 n <= 2077,
 *            u128 n <= 8253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047844:  Patrick De Geest's "Generations" array read by
   antidiagonals.
   Recurrence:
     a(n,1) = n‑th prime
     a(1,k+1) = a(2,k)
     a(n,k+1) = a(n-1,k) + a(n+1,k)
   a(n) is obtained by reading that array along antidiagonals,
   with offset = 1 (so a(1) is the first term). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* statically‑allocated cache for primes and the work table */
static uint64_t *primes      = NULL;   /* list of primes */
static size_t   prime_cnt    = 0;      /* number of stored primes  */
static size_t   prime_cap    = 0;      /* allocated capacity      */

static seq_t *table          = NULL;   /* flattened table (1‑based) */
static idx_t  tbl_size       = 0;      /* current side length of table */

/* ------------------------------------------------------------------ */
/* Prime utilities ---------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need) return;

    /* grow the prime array if necessary */
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap;
    while (new_cap < (size_t)need) new_cap <<= 1;
    if (new_cap > prime_cap) {
        uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        prime_cap = new_cap;
    }

    uint64_t candidate = (prime_cnt == 0) ? 2ULL : primes[prime_cnt - 1] + 1ULL;
    for (; prime_cnt < (size_t)need; ++candidate) {
        bool is_prime = true;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[prime_cnt++] = candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Table building ----------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void
ensure_table(idx_t S)
{
    if (tbl_size >= S) return;

    /* free old table */
    if (table) free(table);

    size_t sz = (S + 1) * (S + 1);
    table = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!table) exit(EXIT_FAILURE);
    tbl_size = S;

    /* make sure we have enough primes */
    ensure_primes(S);

    /* helper for cell indexing */
    #define IDX(i,j) (((i) * (S + 1)) + (j))

    /* column 1: first column is the primes */
    for (idx_t i = 1; i <= S; ++i) {
        table[IDX(i,1)] = (seq_t)primes[i - 1];
    }

    /* remaining columns */
    for (idx_t k = 2; k <= S; ++k) {
        idx_t max_row = S - k + 1;          /* last needed row in this column */

        /* a(1,k) = a(2,k-1) */
        if (max_row >= 1)
            table[IDX(1,k)] = table[IDX(2,k-1)];

        /* a(n,k) for n = 2 .. max_row */
        for (idx_t n = 2; n <= max_row; ++n) {
            table[IDX(n,k)] =
                table[IDX(n-1,k-1)] + table[IDX(n+1,k-1)];
        }
    }

    #undef IDX
}

/* ------------------------------------------------------------------ */
/* Public function ---------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1, but guard anyway */

    /* find minimal S such that S*(S+1)/2 >= n */
    idx_t S = 1;
    while (((unsigned __int128)S * (S + 1) / 2) < n) ++S;

    /* index within the antidiagonal */
    idx_t prev_tri = (S - 1) * S / 2;      /* triangular number of previous diag */
    idx_t j = n - prev_tri;                /* 1‑based position in current diagonal */
    idx_t row = S - j + 1;                 /* i‑coordinate in the array */
    idx_t col = j;                         /* k‑coordinate */

    ensure_table(S);
    return table[row * (tbl_size + 1) + col];
}
