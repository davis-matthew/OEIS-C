/*
 * A144308  Triangle read by rows: prime numbers p(n) along left edge (n, 1)
 *          and p(n - 1) along right edge (n, n), with (n, k) = core((n - 1, k
 *          - 1) + (n - 1, k)) for 1 < k < n when n > 2.
 * https://oeis.org/A144308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: triangle read by rows
   T(1,1)                   = 2
   T(n,1)                   = p(n)             (nth prime)
   T(n,n)                   = p(n-1)
   for 1<k<n:  T(n,k)      = core( T(n-1,k-1) + T(n-1,k) )
   core(x)  = x divided by its largest square factor
   the sequence a(n) is the flattening of the triangle read row‑by‑row. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- prime sieve (once) ---------- */

static idx_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static bool prime_init    = false;

static void init_primes(void)
{
    if (prime_init) return;
    prime_init = true;

    /* 2 million is enough for the 10 000th prime (≈104 729) */
    const idx_t LIMIT = 2000000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    prime_list = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
    size_t cnt = 0;

    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            prime_list[cnt++] = i;
            if (i <= LIMIT / i)
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
        }
    }
    prime_list = (idx_t*)realloc(prime_list, cnt * sizeof(idx_t));
    prime_cnt = cnt;
    free(is_comp);
}

/* ---------- squarefree core ---------- */

static seq_t core_func(seq_t x)
{
    seq_t res = 1;
    idx_t v = (idx_t)x;
    for (size_t i = 0; i < prime_cnt && (unsigned __int128)prime_list[i] * prime_list[i] <= v; ++i)
    {
        idx_t p = prime_list[i];
        if (v % p == 0)
        {
            int parity = 0;
            do
            {
                v /= p;
                parity ^= 1;
            } while (v % p == 0);
            if (parity)
                res = res * (seq_t)p;
        }
    }
    if (v > 1)   /* remaining prime factor with odd exponent */
        res = res * (seq_t)v;
    return res;
}

/* ---------- helper: find row index (T(r) >= n) ---------- */

static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, guard anyway */

    init_primes();

    /* locate row r and column k (1‑based) */
    idx_t r = find_row(n);
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_tri);

    /* allocate the first row */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t));
    prev[0] = (seq_t)prime_list[0];   /* row 1: 2 */

    /* build rows sequentially up to row r */
    for (idx_t cur = 2; cur <= r; ++cur)
    {
        seq_t *curr = (seq_t*)malloc(cur * sizeof(seq_t));
        curr[0]           = (seq_t)prime_list[cur - 1];      /* left edge */
        curr[cur - 1]     = (seq_t)prime_list[cur - 2];      /* right edge */

        for (idx_t j = 1; j + 1 < cur; ++j)
        {
            seq_t sum = prev[j - 1] + prev[j];
            curr[j] = core_func(sum);
        }

        free(prev);
        prev = curr;
    }

    seq_t ans = prev[k - 1];

    free(prev);
    return ans;
}
