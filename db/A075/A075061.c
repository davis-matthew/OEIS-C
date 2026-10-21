/*
 * A075061  Triangle in A075059 read by rows.
 * https://oeis.org/A075061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula for A075061 (flattened triangle from A075059):
 *   Let k be the least integer such that k(k+1)/2 ≥ n.
 *   Then  a(n) = lcm(1,2,…,k) + 1 + (n – k(k‑1)/2 – 1).
 *   The row k of the triangle consists of the numbers
 *        lcm(1..k)+1 , lcm(1..k)+2 , … , lcm(1..k)+k .
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- small prime sieve (up to 2000) ---------- */
#define PRIME_LIMIT 2000
static idx_t  prime_arr[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool   primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool is_comp[PRIME_LIMIT + 1] = { false };
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_arr[prime_cnt++] = (idx_t)i;
            if ((idx_t)i * (idx_t)i <= (idx_t)PRIME_LIMIT) {
                for (idx_t j = (idx_t)i * (idx_t)i; j <= (idx_t)PRIME_LIMIT; j += (idx_t)i)
                    is_comp[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ---------- lcm(1..k)+1 -------------------------------- */
static seq_t lcm_plus_one(idx_t k)
{
    if (k < 1) return (seq_t)2;          /* safety, not used for k>=1 */
    init_primes();

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= k; ++i) {
        seq_t p    = (seq_t)prime_arr[i];
        seq_t power = p;
        while (power * p <= (seq_t)k)
            power = power * p;
        result = result * power;
    }
    return result + (seq_t)1;
}

/* ---------- main sequence function ----------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* find k such that k(k+1)/2 >= n */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    /* previous rows total */
    idx_t prev = (k - 1) * k / 2;

    /* zero‑based position within the row */
    idx_t pos = n - prev - 1;

    seq_t base = lcm_plus_one(k);
    return base + (seq_t)pos;
}
