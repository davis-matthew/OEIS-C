/*
 * A206736  Number of permutations of length n which avoid the patterns 1234,
 *          4321.
 * https://oeis.org/A206736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of permutations of length n avoiding * the
 *            patterns 1234 and 4321
 * Range:     u8 n <= 5, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return 1 if perm[0..n-1] contains an increasing or decreasing
 * subsequence of length 4, otherwise return 0. */
static int has_pattern(const seq_t *perm, idx_t n)
{
    idx_t i,j,k,l;
    for(i=0;i+3<n;i++)
        for(j=i+1;j+2<n;j++)
            for(k=j+1;k+1<n;k++)
                for(l=k+1;l<n;l++){
                    seq_t a=perm[i], b=perm[j], c=perm[k], d=perm[l];
                    if (a<b && b<c && c<d) return 1;
                    if (a>b && b>c && c>d) return 1;
                }
    return 0;
}

/* Recursive backtracking to generate all permutations of size n.
 * `perm` holds the current permutation,
 * `used` marks which numbers have been placed,
 * `depth` is the current position,
 * `cnt` counts permutations avoiding the patterns. */
static void permute(idx_t n, seq_t *perm, unsigned char *used,
                    idx_t depth, seq_t *cnt)
{
    if (depth == n){
        if (!has_pattern(perm, n))
            (*cnt)++;
        return;
    }
    for(idx_t i=0;i<n;i++){
        if (!used[i]){
            used[i] = 1;
            perm[depth] = i+1;
            permute(n, perm, used, depth+1, cnt);
            used[i] = 0;
        }
    }
}

/* Count permutations of length n avoiding 1234 and 4321. */
static seq_t count_avoiding(idx_t n)
{
    if (n <= 1) return (seq_t)1; /* a(0)=1, a(1)=1 */
    seq_t *perm = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    if (!perm) return 0;
    unsigned char *used = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    if (!used){ free(perm); return 0; }
    for(idx_t i=0;i<n;i++) used[i] = 0;
    seq_t counter = 0;
    permute(n, perm, used, 0, &counter);
    free(perm);
    free(used);
    return counter;
}

/* Cache of values for n = 0..9. */
static seq_t cache[10];
static int cache_init = 0;

static void init_cache(void)
{
    if (cache_init) return;
    for(idx_t n=0;n<=9;n++){
        cache[n] = count_avoiding(n);
    }
    cache_init = 1;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n >= 10) return (seq_t)0;
    init_cache();
    return cache[n];
}
