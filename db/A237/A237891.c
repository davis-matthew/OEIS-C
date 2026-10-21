/*
 * A237891  Number of (n+1)X(1+1) 0..3 arrays with the maximum minus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d=0..3} sum_{i=0..15} sum_{j=0..15} (A_d^n)[i][j]
 *            where A_d[i][j] = 1 if, for rows i and j (each two digits 0..3),
 *            the difference max(row_i,row_j) – upper_median(row_i,row_j)
 *            equals d. The algorithm uses dynamic programming on the 16×16
 *            adjacency matrices for the four possible values...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int diff_of_rows(int a,int b,int c,int d)
{
    int v[4] = {a,b,c,d};
    /* simple insertion sort for four elements */
    for(int i=0;i<4;i++)
        for(int j=i+1;j<4;j++)
            if(v[i]>v[j]){ int t=v[i]; v[i]=v[j]; v[j]=t;}
    return v[3]-v[2];   /* max – upper median (third smallest) */
}

seq_t a(idx_t n)
{
    /* adjacency[d][prev][next] == 1 iff the two rows satisfy the
       required difference d; 0 ≤ d ≤ 3, 0 ≤ prev,next ≤ 15. */
    static unsigned char adjacency[4][16][16];
    static int initialized = 0;

    if(!initialized)
    {
        for(int prev=0; prev<16; ++prev)
        {
            int a = prev>>2, b = prev&3;
            for(int next=0; next<16; ++next)
            {
                int c = next>>2, d = next&3;
                int diff = diff_of_rows(a,b,c,d);
                adjacency[diff][prev][next] = 1;
            }
        }
        initialized = 1;
    }

    seq_t total = 0;
    seq_t dp_cur[16], dp_next[16];

    for(int d=0; d<4; ++d)
    {
        /* start with a single row: every state is possible */
        for(int i=0; i<16; ++i) dp_cur[i] = 1;

        /* perform n transitions (n steps) to obtain n+1 rows */
        for(idx_t step=0; step<n; ++step)
        {
            for(int j=0; j<16; ++j) dp_next[j] = 0;

            for(int i=0; i<16; ++i)
            {
                seq_t val = dp_cur[i];
                if(val==0) continue;
                for(int j=0; j<16; ++j)
                    if(adjacency[d][i][j])
                        dp_next[j] += val;
            }

            for(int j=0; j<16; ++j) dp_cur[j] = dp_next[j];
        }

        /* sum over all ending rows */
        seq_t sum = 0;
        for(int j=0; j<16; ++j) sum += dp_cur[j];
        total += sum;
    }

    return total;
}
