/*
 * A196205  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,3,4,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-4) + 3*a(n-5) - 2*a(n-6) +
 *            2*a(n-7) - 2*a(n-8) + a(n-9) Initial values (offset 1): a(1)=4,
 *            a(2)=8, a(3)=7, a(4)=14, a(5)=17, a(6)=27, a(7)=42, a(8)=58,
 *            a(9)=94
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[20001];           /* 1‑based cache, capacity ≥ 10000 */
static idx_t computed_max = 0;       /* highest n computed so far */

/* Compute the n-th term of OEIS sequence A196205 */
seq_t a(idx_t n)
{
    /* Return stored value if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Initialize the first nine terms exactly once */
    if (computed_max == 0) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)17;
        cache[6] = (seq_t)27;
        cache[7] = (seq_t)42;
        cache[8] = (seq_t)58;
        cache[9] = (seq_t)94;
        computed_max = 9;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (computed_max + 1 <= 9 ? 10 : computed_max + 1); i <= n; ++i) {
        cache[i] = 3*cache[i-1]          /* +3*a(n‑1)            */
                  - 2*cache[i-2]         /* -2*a(n‑2)            */
                  - 2*cache[i-4]         /* -2*a(n‑4)            */
                  + 3*cache[i-5]         /* +3*a(n‑5)            */
                  - 2*cache[i-6]         /* -2*a(n‑6)            */
                  + 2*cache[i-7]         /* +2*a(n‑7)            */
                  - 2*cache[i-8]         /* -2*a(n‑8)            */
                  + cache[i-9];          /* +a(n‑9)              */
    }

    computed_max = n;
    return cache[n];
}
