/*
 * A105118  a(n) = prime(n) + prime(n+k) mod 3, where k = (1/2)*(prime(n+1) -
 *          prime(n)).
 * https://oeis.org/A105118
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + prime(n + k) mod 3, where k = (prime(n+1) –
 *            prime(n)) / 2 (integer division, offset 2)
 * Range:     u8 n <= 29, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------------- static prime storage ---------------------------- */
static uint64_t *primes = NULL;   /* zero‑based array of known primes   */
static idx_t primes_cnt = 0;      /* number of primes currently stored */

/* Ensure that at least 'need' primes are stored in the array */
static void ensure_primes(idx_t need)
{
    if (need <= primes_cnt) return;

    /* Allocate or grow the array */
    if (primes_cnt == 0) {
        primes = (uint64_t *)malloc(256 * sizeof(uint64_t));
    } else if (primes_cnt < 256) {        /* capacity already 256 */
        /* nothing to do */
    } else {
        /* double capacity until large enough */
        size_t cap = primes_cnt;
        while (cap < need) cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
        if (tmp == NULL) abort();
        primes = tmp;
    }

    /* Start searching for primes after the last known one */
    uint64_t cand = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    if (cand <= 2) cand = 2;

    while (primes_cnt < need) {
        /* Skip even numbers > 2 */
        if (cand > 2 && (cand & 1) == 0) { cand++; continue; }

        /* Trial division by known primes */
        int is_prime = 1;
        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t p2 = p * p;
            if (p2 > cand) break;
            if ((cand % p) == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            primes[primes_cnt++] = cand;
        }
        cand++;
    }
}

/* ----------------- sequence implementation -------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* undefined below offset */

    /* We need primes up to at least n+50 to be safe for k */
    ensure_primes(n + 50);

    /* Prime indices are 1‑based in the OEIS definition */
    uint64_t p_n   = primes[n - 1];          /* prime(n)     */
    uint64_t p_np1 = primes[n];              /* prime(n+1)   */

    /* Compute k = (prime(n+1) – prime(n)) / 2 */
    uint64_t diff = p_np1 - p_n;              /* always non‑negative   */
    idx_t k = diff >> 1;                      /* integer division by 2 */

    /* Ensure we have prime(n + k) */
    idx_t needed_index = n + k;               /* 1‑based index   */
    ensure_primes(needed_index);

    uint64_t p_nk = primes[needed_index - 1]; /* prime(n + k)    */

    /* Compute (prime(n) + prime(n+k)) mod 3 */
    seq_t sum = (seq_t)p_n + (seq_t)p_nk;
    return sum % 3;
}
