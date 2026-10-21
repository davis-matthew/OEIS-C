/*
 * A081768  Number of k, 1<=k<=n, such that C(n,k) divides C(2n,2k).
 * https://oeis.org/A081768
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

/* Compute a(n) = #{ 1 ≤ k ≤ n | C(n,k) divides C(2n,2k) }.
   Uses Kummer's theorem: v_p(C(n,k)) = #  carries in base‑p addition of k and n‑k.
   For each prime p up to 2n, we pre‑compute v_p(i!) for i = 0…2n and check
   v_p(C(n,k)) ≤ v_p(C(2n,2k)). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* primes up to 20000 (enough for n ≤ 10000). */
static int primes[2300];
static size_t prime_cnt = 0;

/* simple sieve of Eratosthenes */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    const int LIMIT = 20000;
    bool *comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (int i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= LIMIT)
                for (int j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
        }
    }
    free(comp);
    done = true;
}

seq_t a(idx_t n)
{
    init_primes();

    if (n == 0) return (seq_t)0;

    size_t N = (size_t)n;
    uint8_t *ok = (uint8_t*)calloc(N + 1, sizeof(uint8_t));
    for (size_t k = 1; k <= N; ++k) ok[k] = 1;
    size_t remaining = N;

    size_t maxlen = 2 * N;
    int *pref = (int*)calloc(maxlen + 1, sizeof(int));

    for (size_t pi = 0; pi < prime_cnt && primes[pi] <= 2 * (int)n; ++pi) {
        int p = primes[pi];

        /* pref[i] = v_p(i!)  for 0 ≤ i ≤ 2n */
        pref[0] = 0;
        for (size_t i = 1; i <= maxlen; ++i) {
            size_t x = i;
            int e = 0;
            while ((x % p) == 0) {
                ++e;
                x /= p;
            }
            pref[i] = pref[i - 1] + e;
        }

        for (size_t k = 1; k <= N; ++k) {
            if (!ok[k]) continue;
            int left  = pref[N] - pref[k] - pref[N - k];
            int right = pref[2 * N] - pref[2 * k] - pref[2 * N - 2 * k];
            if (left > right) {
                ok[k] = 0;
                --remaining;
            }
        }
        if (remaining == 0) break;
    }

    free(pref);
    free(ok);

    return (seq_t)remaining;
}
