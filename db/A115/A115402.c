/*
 * A115402  Difference between 3-almostprime(n) and 3-almostprime(n+3).
 * https://oeis.org/A115402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A115402
   a(n) = A014612(n+3) – A014612(n)
   where A014612(k) is the k‑th 3‑almost prime (a number with exactly
   three prime factors, counted with multiplicity). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---- helper to grow a seq_t array ------------------------------------- */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ---- static storage for the list of 3‑almost primes ------------------- */
static seq_t *three = NULL;  /* array of 3‑almost primes (0‑based)     */
static size_t three_len  = 0; /* number of elements stored             */
static size_t three_cap  = 0; /* current capacity                      */

/* ---- build the first required 3‑almost primes -------------------------- */
static void init_three_almost_primes(size_t required)
{
    if (three_len >= required) return;                /* already enough  */

    /* If we already have an array, free it – we rebuild with a larger
       limit when necessary.  The 500 000 bound is sufficient for the
       first 10 000 terms (so this function runs only once in normal use). */
    free(three);
    three = NULL;
    three_len = 0;
    three_cap = 0;

    const size_t limit = 500000;       /* safe upper bound for needed primes */
    uint8_t *omega = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!omega) exit(1);

    /* linear sieve to compute ω(n) – number of prime factors (with
       multiplicity) for every n up to limit. */
    uint32_t *primes = (uint32_t*)malloc(100000 * sizeof(uint32_t));
    if (!primes) { free(omega); exit(1); }
    size_t primes_len = 0;

    for (size_t i = 2; i <= limit; ++i)
    {
        if (!omega[i])        /* i is prime */
        {
            omega[i] = 1;
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len; ++j)
        {
            uint32_t p = primes[j];
            size_t ip = i * p;
            if (ip > limit) break;
            omega[ip] = omega[i] + 1;
            if (i % p == 0) break;
        }
    }

    /* collect numbers whose ω(n) == 3 */
    for (size_t i = 2; i <= limit; ++i)
    {
        if (omega[i] == 3)
        {
            grow_seq_array(&three, &three_cap, three_len + 1);
            three[three_len++] = (seq_t)i;
        }
    }

    free(omega);
    free(primes);

    /* If we still have fewer than required values (very unlikely for n≤10⁴),
       we could double the limit and rebuild.  For the constraints of the
       task this never happens. */
}

/* ---- sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined by OEIS      */

    /* Ensure we have the 3‑almost primes up to rank n+3. */
    init_three_almost_primes((size_t)(n + 3));

    /* indices:  three[k] holds the (k+1)-st 3‑almost prime.          */
    size_t start = (size_t)(n - 1);           /* rank n  → array pos n‑1   */
    size_t end   = start + 3;                 /* rank n+3 → array pos n+2   */
    return three[end] - three[start];
}
