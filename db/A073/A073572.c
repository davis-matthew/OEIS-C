/*
 * A073572  Largest k such that the arithmetic mean of
 *          phi(n),phi(n+1),....,phi(n+x) is an integer for any x 0<=x<=k.
 * https://oeis.org/A073572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{k | for all x with 0 ≤ x ≤ k, (φ(n)+…+φ(n+x))/(x+1)
 *            is an integer} The algorithm pre‑computes φ on the fly with a
 *            linear sieve, then scans forward from n, accumulating a running
 *            sum. The largest k for which the sum is divisible by (current
 *            length) is the answer
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed headers for C11 and C++17 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------   static storage for the φ sieve   ---------- */
static seq_t *phi_arr   = NULL;   /* φ[0..]   (φ[0] unused) */
static idx_t  phi_limit = 0;      /* highest index currently computed */
static bool *  is_comp   = NULL;   /* composite flag array */
static idx_t * primes_arr = NULL; /* list of primes found */
static size_t prime_cnt  = 0;      /* number of stored primes */
static size_t prime_cap  = 0;      /* capacity of the primes array */

/* ----------   ensure φ values up to 'limit' (inclusive)   ---------- */
static void ensure_phi(idx_t limit)
{
    if (limit <= phi_limit)
        return;                           /* already enough */

    /* increase sizes to accommodate indices up to 'limit' */
    size_t new_cap = (size_t)limit + 1;    /* 0 .. limit inclusive */

    /* phi array */
    phi_arr = (seq_t *)realloc(phi_arr, new_cap * sizeof(seq_t));
    if (!phi_arr) exit(1);

    /* composite flag array */
    is_comp = (bool *)realloc(is_comp, new_cap * sizeof(bool));
    if (!is_comp) exit(1);

    /* clear new slots and initialize φ[0] to 0 (unused) */
    for (idx_t i = phi_limit + 1; i < new_cap; ++i) {
        phi_arr[i] = 0;
        is_comp[i] = false;
    }

    /* φ(1) = 1 (if we are past 1) */
    if (phi_limit < 1 && limit >= 1)
        phi_arr[1] = 1;

    /* linear sieve from max(phi_limit+1, 2) to limit */
    idx_t start = (phi_limit < 2 ? 2 : phi_limit + 1);
    for (idx_t i = start; i <= limit; ++i) {
        if (!is_comp[i]) {                                 /* i is prime */
            /* grow prime list if necessary */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes_arr = (idx_t *)realloc(primes_arr, prime_cap * sizeof(idx_t));
                if (!primes_arr) exit(1);
            }
            primes_arr[prime_cnt++] = i;
            phi_arr[i] = (seq_t)i - 1;                     /* φ(p) = p-1 */
        }
        /* propagate to multiples */
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes_arr[j];
            idx_t x = i * p;
            if (x > limit)
                break;
            is_comp[x] = true;
            if (i % p == 0) {                              /* p divides i */
                phi_arr[x] = phi_arr[i] * (seq_t)p;        /* φ(i·p) = φ(i)·p   */
                break;
            } else {
                phi_arr[x] = phi_arr[i] * ((seq_t)p - 1);  /* φ(i·p) = φ(i)(p-1)*/
            }
        }
    }
    phi_limit = limit;
}

/* ----------   φ lookup   ---------- */
static inline seq_t phi_value(idx_t n)
{
    return phi_arr[n];   /* n is guaranteed to be <= phi_limit */
}

/* ----------   OEIS sequence entry   ---------- */
seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is offset 1; 0 is never queried, but guard anyway */
        return 0;

    /* be ready to check a few positions beyond n */
    ensure_phi(n + 200);            /* safety margin: rarely enough, but safe */

    seq_t sum = 0;   /* running sum of φ(n)… */
    seq_t k   = 0;   /* current largest k found */

    for (idx_t offset = 0;; ++offset) {
        idx_t idx = n + offset;
        if (idx > phi_limit)
            ensure_phi(idx + 200);        /* extend if required */

        sum += phi_value(idx);

        seq_t m = (seq_t)(offset + 1);   /* length of current interval */
        if ((sum % m) != 0)
            break;                       /* condition fails – stop */

        k = offset;                      /* update largest k */
    }
    return k;
}
