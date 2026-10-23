/*
 * A142712  Primes of the form (p(n)-2*n)/3, where p(n)=n-th prime.
 * https://oeis.org/A142712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime that appears as (p(n) - 2*n)/3, where p(n)
 *            is the n‑th prime. The sequence lists these primes, with
 *            repeats, in the order of the indices n
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Dynamic prime array ---------------------------------------- */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;

static void grow_primes(void)
{
    if (primes_cnt == primes_cap) {
        size_t newcap = primes_cap ? primes_cap * 2 : 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = newcap;
    }
}

static void generate_next_prime(void)
{
    uint64_t cand = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) break;
        ++cand;
    }
    grow_primes();
    primes[primes_cnt++] = cand;
}

/* Ensure that primes[0..need-1] exist */
static void ensure_primes_up_to(idx_t need)
{
    while (primes_cnt < need) generate_next_prime();
}

/* Primality test using stored primes (knows all primes ≤ p_n) */
static bool is_prime_using_list(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ---------- Dynamic sequence array ------------------------------------- */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void grow_seq(void)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq = tmp;
        seq_cap = newcap;
    }
}

/* ---------- Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based */

    static idx_t processed_prime_idx = 0;   /* index of the current p_n */
    while (seq_len < n) {
        ++processed_prime_idx;
        ensure_primes_up_to(processed_prime_idx);

        uint64_t pk = primes[processed_prime_idx - 1];
        int64_t diff = (int64_t)pk - (int64_t)2 * (int64_t)processed_prime_idx;

        if (diff < 0) continue;
        if (diff % 3 != 0) continue;

        uint64_t q = (uint64_t)(diff / 3);
        if (is_prime_using_list(q)) {
            grow_seq();
            seq[seq_len++] = (seq_t)q;
        }
    }
    return seq[n - 1];
}
