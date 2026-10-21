/*
 * A051237  Lexicographically earliest prime pyramid, read by rows.
 * https://oeis.org/A051237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051237 – Lexicographically earliest prime pyramid, read by rows
   Formula: a(n) = row ⌈(n(n+1)/2)⌉[ (n(n+1)/2) - Σ_{k=1}^{⌊(n-1)/2⌋} k ]
   The program builds each row only once, memoises it, and uses a
   simple back‑tracking search that respects the parity alternation
   mandated by the prime‑summing condition.  Sieve of Eratosthenes is
   kept global and enlarged as needed.  */
#include <stdbool.h>
#include <stdlib.h>

/* Maximum row number ever requested (up to 10000 by the problem requirement). */
#if defined(__cplusplus) || defined(_MSC_VER)
#  if _MSC_VER
#    define MAX_ROW  10000ULL
#  else
#    define MAX_ROW  10000ull
#  endif
#else
#  define MAX_ROW  10000ull
#endif

static seq_t *row_cache[MAX_ROW + 1];
static size_t prefix_sum[MAX_ROW + 1];   /* prefix_sum[i] = total elements up to row i  */
static idx_t max_row_built = 0;         /* highest row cached so far */

static bool *prime_sieve = NULL;        /* 1 if prime, 0 otherwise */
static idx_t prime_limit = 0;           /* largest value for which sieve was built */

/* Extend the prime sieve to cover all numbers ≤ 2*limit (so that all
   possible sums of adjacent entries up to 2*limit can be tested). */
static void ensure_prime_sieve(idx_t limit)
{
    idx_t need = 2 * limit;
    if (need <= prime_limit) return;

    bool *new_sieve = (bool *)realloc(prime_sieve, (need + 1) * sizeof(bool));
    if (!new_sieve) exit(1); /* memory failure */
    /* Initialise new part as true */
    for (idx_t i = prime_limit + 1; i <= need; ++i)
        new_sieve[i] = true;
    /* Treat 0 and 1 as non–prime */
    new_sieve[0] = new_sieve[1] = false;

    /* Re‑sieve all numbers up to 'need' */
    for (idx_t i = 2; i * i <= need; ++i)
        if (new_sieve[i])
            for (idx_t j = i * i; j <= need; j += i)
                new_sieve[j] = false;

    prime_sieve = new_sieve;
    prime_limit = need;
}

/* Back‑tracking search for the lexicographically earliest row of length n.
   row[0] = 1, row[n-1] = n are already set before calling this function.
   'used' marks numbers 2..n-1 that are already placed.
   Returns true when a complete row is found. */
static bool build_row_rec(idx_t pos, idx_t n, seq_t *row, bool *used)
{
    if (pos == n - 1) {               /* last position already set to n */
        /* Verify last adjacency (row[n-2] + n). */
        return prime_sieve[(idx_t)row[pos - 1] + n];
    }

    seq_t prev = row[pos - 1];
    /* Determine required parity: odd positions (0‑based even) must be odd,
       even positions (0‑based odd) must be even, because all sums are odd primes. */
    bool need_odd = ((pos & 1) == 0);   /* pos 0 -> odd, pos 1 -> even, etc. */
    for (idx_t cand = 2; cand <= n - 1; ++cand) {
        if (used[cand]) continue;
        if ((cand & 1) != need_odd) continue;            /* wrong parity */
        if (!prime_sieve[(idx_t)prev + cand]) continue;   /* sum not prime */

        used[cand] = true;
        row[pos] = (seq_t)cand;
        if (build_row_rec(pos + 1, n, row, used))
            return true;
        used[cand] = false;   /* backtrack */
    }
    return false;   /* no arrangement for this prefix */
}

/* Construct the row of length n (n ≥ 1) and cache it.  Also update
   the prefix sums. */
static void build_row(idx_t n)
{
    seq_t *row = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!row) exit(1);
    row[0] = (seq_t)1;
    row[n - 1] = (seq_t)n;
    row_cache[n] = row;
    prefix_sum[n] = prefix_sum[n - 1] + n;

    if (n <= 2) return;   /* rows 1 and 2 are trivial */

    /* Prime sieve must cover all possible sums up to 2*n-1 */
    ensure_prime_sieve(n);

    bool *used = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!used) exit(1);
    used[1] = used[n] = true;  /* 1 and n are already used */

    /* Search for the lexicographically earliest remaining entries. */
    build_row_rec(1, n, row, used);
    free(used);
}

/* ----------------------  Main function a()  --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but safety */

    /* Initialise the global structures on first call. */
    if (max_row_built == 0) {
        for (idx_t i = 0; i <= MAX_ROW; ++i)
            row_cache[i] = NULL;
        prefix_sum[0] = 0;
    }

    /* Build rows incrementally until we have enough entries for index n. */
    while (prefix_sum[max_row_built] < (size_t)n) {
        ++max_row_built;
        if (max_row_built > MAX_ROW) exit(1); /* safety */
        if (!row_cache[max_row_built])
            build_row(max_row_built);
    }

    /* Locate the row containing the n‑th element. */
    idx_t row = max_row_built;
    size_t prev_total = prefix_sum[row - 1];
    size_t offset = (size_t)n - prev_total - 1;  /* zero‑based within the row */
    return row_cache[row][offset];
}
